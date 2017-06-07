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

    cam=new VideoCapture(0);
    width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
    height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);
    width=width/2;
    height=height/2;
    cam->set(CV_CAP_PROP_FRAME_WIDTH, width);
    cam->set(CV_CAP_PROP_FRAME_HEIGHT, width);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

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
            //TRAITEMENT IMAGE
            if(calibrationOk){
               TI.reconstruction(calibrationImg,image);
            } else {
                calibrationImg = image;
            }
        }
    }
    //TESTS
    //TI.reconstruction(calibration, capture);
    //FIN TESTS
}



void MainWindow::on_pushButton_clicked()
{

    if ( ui->checkAuto->isChecked()){

    }else{
        Point maxLoc;
        maxLoc.x=centreImg.x;
        maxLoc.y=centreImg.y;

                // HAUT GAUCHE
                Mat Img1 =image;
                Mat Img2 =image;
                Img1 =Img1.colRange(0,maxLoc.x);
                Img1=Img1.rowRange(0,maxLoc.y);
                namedWindow("Haut GAUCHE",1);
                imshow("Haut GAUCHE", Img1);
                // HAUT DROIT

                Img2=Img2.colRange(maxLoc.x,width);
                Img2=Img2.rowRange(0,maxLoc.y);
                namedWindow("HAUT DROIT",1);
                imshow("HAUT DROIT", Img2);
                // BAS GAUCHE
                Mat Img3 =image;
                Img3=Img3.colRange(0,maxLoc.x);
                Img3=Img3.rowRange(maxLoc.y,height);
                namedWindow("BAS GAUCHE",1);
                imshow("BAS GAUCHE", Img3);
                // BAS DROIT
                Mat Img4 =image;
                Img4=Img4.colRange(maxLoc.x,width);
                Img4=Img4.rowRange(maxLoc.y,height);
                namedWindow("BAS DROIT",1);
                imshow("BAS DROIT", Img4);
              // QString instrument= detectionInstrument(Img4,maxLoc);
              // qDebug()<<instrument;
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

     PositionX=&lPositionX;
     PositionY=&lPositionY;
     *PositionX=*PositionX-90;
     *PositionY=*PositionY-20;
     qDebug()<<"x1: "<<*PositionX<<" et y1: "<<*PositionY;
     qDebug()<<"xmax: "<<width<<" et ymax: "<<height;
    // ok=false;

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
}

void MainWindow::on_imgCalib_clicked()
{
    if (ui->imgCalib->isChecked()){
        calibrationOk = true;
    } else{
        calibrationOk = false;
    }

}

void MainWindow::decoupageImage(){
    Point maxLoc;
    maxLoc.x=*PositionX;
    maxLoc.y=*PositionY;
    cvtColor(image,image,CV_BGR2RGB);
    // HAUT GAUCHE
    Mat Img1 =image;
    Img1 =Img1.colRange(0,maxLoc.x);
    Img1=Img1.rowRange(0,maxLoc.y);
    namedWindow("Haut GAUCHE",1);
    imshow("Haut GAUCHE", Img1);
    // HAUT DROIT
    Mat Img2 =image;
    Img2=Img2.colRange(maxLoc.x,width);
    Img2=Img2.rowRange(0,maxLoc.y);
    namedWindow("HAUT DROIT",1);
    imshow("HAUT DROIT", Img2);
    // BAS GAUCHE
    Mat Img3 =image;
    Img3=Img3.colRange(0,maxLoc.x);
    Img3=Img3.rowRange(maxLoc.y,height);
    namedWindow("BAS GAUCHE",1);
    imshow("BAS GAUCHE", Img3);
    // BAS DROIT
    Mat Img4 =image;
    Img4=Img4.colRange(maxLoc.x,width);
    Img4=Img4.rowRange(maxLoc.y,height);
    namedWindow("BAS DROIT",1);
    imshow("BAS DROIT", Img4);
}
