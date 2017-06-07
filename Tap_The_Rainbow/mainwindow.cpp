#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <QDir>
#include <QMouseEvent>
#include <QThread>

using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox_2->setEnabled(false);
    ui->groupBox->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->checkAuto->setEnabled(true);
    ui->instrument1->setEnabled(false);
    ui->instrument2->setEnabled(false);
    ui->instrument3->setEnabled(false);
    ui->instrument4->setEnabled(false);

    imagette=false;
    cam=new VideoCapture(0);
    width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
    height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);
    width=width/2;
    height=height/2;
    cam->set(CV_CAP_PROP_FRAME_WIDTH, width);
    cam->set(CV_CAP_PROP_FRAME_HEIGHT, width);

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(soloT, SIGNAL(timeout()), this, SLOT(modeSolo()));
    connect(multiT, SIGNAL(timeout()), this, SLOT(modeMulti()));

   /* calibration = imread("C:/Users/Arnaud/Pictures/Camera Roll/IMG 2.0/calibration1.jpg");
    capture = imread("C:/Users/Arnaud/Pictures/Camera Roll/IMG 2.0/rouge3.jpg");
    cv::resize(calibration,calibration,Size(),0.5,0.5);
    cv::resize(capture,capture,Size(),0.5,0.5);*/
    calibrationOk = false;
    timer->start(50);  //1000

}

MainWindow::~MainWindow()
{
    delete ui;
    delete cam;
}
void MainWindow::modeSolo(){
    cvtColor(image,image,CV_BGR2RGB);
    if(calibrationOk){
       TI.reconstruction(calibrationImg,image);
    }
}

void MainWindow::modeMulti(){
    //decoupageImage();
    Point maxLoc;
    maxLoc.x=centreImg.x;
    maxLoc.y=centreImg.y;
    cvtColor(image,image,CV_BGR2RGB);
    // HAUT GAUCHE
    Img1T =image;
    Img1T =Img1T.colRange(0,maxLoc.x);
    Img1T=Img1T.rowRange(0,maxLoc.y);

    // HAUT DROIT
    Img2T =image;
    Img2T=Img2T.colRange(maxLoc.x,width);
    Img2T=Img2T.rowRange(0,maxLoc.y);

    // BAS GAUCHE
    Img3T =image;
    Img3T=Img3T.colRange(0,maxLoc.x);
    Img3T=Img3T.rowRange(maxLoc.y,height);

    // BAS DROIT
    Img4T =image;
    Img4T=Img4T.colRange(maxLoc.x,width);
    Img4T=Img4T.rowRange(maxLoc.y,height);

    //Test d'affichage
   /* namedWindow("Haut GAUCHE",1);
    imshow("Haut GAUCHE", Img1T);
    namedWindow("HAUT DROIT",1);
    imshow("HAUT DROIT", Img2T);
    namedWindow("BAS GAUCHE",1);
    imshow("BAS GAUCHE", Img3T);
    namedWindow("BAS DROIT",1);
    imshow("BAS DROIT", Img4T);*/
    //fin test

    // TRAITEMENT si l'instrument est selectionné
    if(instrument1){
        TI.reconstruction(Img1,Img1T);
    }
    if(instrument2){
        TI.reconstruction(Img2,Img2T);
    }
    if(instrument3){
        TI.reconstruction(Img3,Img3T);
    }
    if(instrument4){
        TI.reconstruction(Img4,Img4T);
    }
}
void MainWindow::update(){
    if (cam->isOpened()) {
        if (cam->read(image)) {   // Capture a frame
            // Flip to get a mirror effect
            flip(image,image,1);
            // Invert Blue and Red color channels
            cvtColor(image,image,CV_BGR2RGB);
            // Convert to Qt image
            QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
            // Display on label
            ui->camFrame->setPixmap(QPixmap::fromImage(img));
            // Resize the label to fit the image
            ui->camFrame->resize(ui->camFrame->pixmap()->size());
        }
    }
    //TESTS
    //TI.reconstruction(calibration, capture);
    //FIN TESTS
}



void MainWindow::on_pushButton_clicked()
{
    if (ui->pushButton->text()=="Reset Calibration"){ //RESET CALIBRATION
     ui->checkAuto->setChecked(false);
     ui->checkAuto->setEnabled(true);
     ui->checkBox->setEnabled(true);
     ui->checkBox->setChecked(false);
     ui->instrument1->setEnabled(false);
     ui->instrument2->setEnabled(false);
     ui->instrument3->setEnabled(false);
     ui->instrument4->setEnabled(false);
     ui->pushButton->setText("Valider");
     multiT->stop();
     play=false;
    }else{ //VALIDER
     ui->checkAuto->setEnabled(false);
     ui->checkBox->setEnabled(false);
     ui->instrument1->setEnabled(true);
     ui->instrument2->setEnabled(true);
     ui->instrument3->setEnabled(true);
     ui->instrument4->setEnabled(true);
     ui->instrument1->setChecked(false);
     ui->instrument2->setChecked(false);
     ui->instrument3->setChecked(false);
     ui->instrument4->setChecked(false);
     ui->pushButton->setText("Reset Calibration");
     multiT->start(1000);
     play=true;
    }
 }


QString  MainWindow::detectionInstrument(Mat image,Point centre){
    Mat resultImage;
    Mat templateImage;
    int i =0;
    double minVal; double maxVal; Point minLoc; Point maxLoc;

    while(maxVal>0.95){
        switch(i){ //Choix de l'instrument
        case 0 :  templateImage = imread("./debug/imagesSrc/batterie_ID.jpg");
            break;
        case 1 : templateImage = imread("./debug/imagesSrc/harpe.jpg");
            break;
        case 2 : templateImage = imread("./debug/imagesSrc/piano_ID.jpg");
            break;
        case 3 : templateImage = imread("./debug/imagesSrc/flute.jpg");
            break;
        default :
            break;
        }
            int result_cols =  image.cols - templateImage.cols + 1;
            int result_rows = image.rows - templateImage.rows + 1;
            resultImage.create( result_cols, result_rows, CV_32FC1 );
            matchTemplate( image, templateImage, resultImage, TM_CCORR_NORMED );
            minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
            i=+1;
            }
    //détermination de l'instrument
    if(maxLoc.x<centre.x){
        if(maxLoc.y<centre.y){
            QString res= "Piano 1";
            return res;
        }else{
            QString res= "Pas Piano 1";
            return res;
        }

    }else{
        if(maxLoc.y<centre.y){
        QString res= "Batterie";
        return res;
        }else{
            QString res= "Batterie";
            return res;

        }
    }
}

void  MainWindow::mousePressEvent(QMouseEvent *event) {
    if(manu){ //uniquement si le découpage manuel est activé
     int lPositionX = event->x();
     int lPositionY = event->y();
     key=true;
     PositionX=&lPositionX;
     PositionY=&lPositionY;
     *PositionX=*PositionX-100;
     *PositionY=*PositionY-20;
     centreImg.x=*PositionX;
     centreImg.y=*PositionY;
    if(*PositionX>0 && *PositionX<320){
        if(*PositionY>0 && *PositionY<240){
            decoupageImage();
        }
    }
    }
}

void MainWindow::on_checkAuto_clicked()
{
    manu=true;
    ui->pushButton->setEnabled(true);
}


void MainWindow::decoupageImage(){
   if (multi){
    Point maxLoc;
    maxLoc.x=*PositionX;
    maxLoc.y=*PositionY;
    cvtColor(image,image,CV_BGR2RGB);
    if (play){

    }  else{
    // HAUT GAUCHE
    Img1 =image;
    Img1 =Img1.colRange(0,maxLoc.x);
    Img1=Img1.rowRange(0,maxLoc.y);

    // HAUT DROIT
    Img2 =image;
    Img2=Img2.colRange(maxLoc.x,width);
    Img2=Img2.rowRange(0,maxLoc.y);

    // BAS GAUCHE
    Img3 =image;
    Img3=Img3.colRange(0,maxLoc.x);
    Img3=Img3.rowRange(maxLoc.y,height);

    // BAS DROIT
    Img4 =image;
    Img4=Img4.colRange(maxLoc.x,width);
    Img4=Img4.rowRange(maxLoc.y,height);
    }
    if (imagette && manu && key){
     namedWindow("Haut GAUCHE",1);
     imshow("Haut GAUCHE", Img1);
     namedWindow("HAUT DROIT",1);
     imshow("HAUT DROIT", Img2);
     namedWindow("BAS GAUCHE",1);
     imshow("BAS GAUCHE", Img3);
     namedWindow("BAS DROIT",1);
     imshow("BAS DROIT", Img4);
    }

   }
}

void MainWindow::on_butonMulti_clicked()
{
    ui->groupBox->setEnabled(false);
    ui->groupBox_2->setEnabled(true);
    ui->butonJoueur->setEnabled(true);
    ui->butonMulti->setEnabled(false);
    multi=true;
    soloT->stop();
}

void MainWindow::on_butonJoueur_clicked()
{
    ui->groupBox->setEnabled(true);
    ui->groupBox_2->setEnabled(false);
    ui->butonJoueur->setEnabled(false);
    ui->butonMulti->setEnabled(true);
    multi=false;
    multiT->stop();
}

void MainWindow::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked()){
        imagette=true;
        if (imagette && manu && key){
         cvtColor(image,image,CV_BGR2RGB);
         namedWindow("Haut GAUCHE",1);
         imshow("Haut GAUCHE", Img1);
         namedWindow("HAUT DROIT",1);
         imshow("HAUT DROIT", Img2);
         namedWindow("BAS GAUCHE",1);
         imshow("BAS GAUCHE", Img3);
         namedWindow("BAS DROIT",1);
         imshow("BAS DROIT", Img4);
        }
    }else{
        imagette=false;
    }
}

void MainWindow::on_instrument1_clicked()
{
    if (ui->instrument1->isChecked()){
        instrument1=true;
    }else{
        instrument1=false;
    }
}

void MainWindow::on_instrument3_clicked()
{
    if (ui->instrument3->isChecked()){
        instrument3=true;
    }else{
        instrument3=false;
    }
}

void MainWindow::on_instrument2_clicked()
{
    if (ui->instrument2->isChecked()){
        instrument2=true;
    }else{
        instrument2=false;
    }
}

void MainWindow::on_instrument4_clicked()
{
    if (ui->instrument4->isChecked()){
        instrument4=true;
    }else{
        instrument4=false;
    }
}

void MainWindow::on_calibration_clicked()
{
    if(ui->calibration->text()=="Calibration"){
        calibrationImg = image;
        calibrationOk=true;
        ui->calibration->setText("Refaire calibration");
        soloT->start(1000);
    }else{
        calibrationOk=false;
        ui->calibration->setText("Calibration");
        soloT->stop();
    }
}
