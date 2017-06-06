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
    int width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
    int height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);

    if(!cam->isOpened())  // check if we succeeded
    {
        ui->camInfo->setText("Error openning the default camera !");
    }
    else
    {
        ui->camInfo->setText(QString("Video ok, image size is %1x%2 pixels").arg(width).arg(height));
    }

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    calibration = imread("C:/Users/Arnaud/Pictures/Camera Roll/IMG 2.0/calibration1.jpg");
    capture = imread("C:/Users/Arnaud/Pictures/Camera Roll/IMG 2.0/vert1.jpg");
    cv::resize(calibration,calibration,Size(),0.4,0.4);
    cv::resize(capture,capture,Size(),0.4,0.4);

    timer->start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete cam;
}
void MainWindow::update(){

    TI.reconstruction(calibration, capture);

    // Definition of the template rectangle
   // int templateWidth=80;
   // int templateHeight=80;


    if (cam->isOpened()) {
        Mat image;
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
            //qDebug()<<ui->camFrame->pixmap()->size();
        }
        else {
            ui->camInfo->setText("Error capturing the frame");
        }
    }
}



void MainWindow::on_pushButton_clicked()
{
    if ( ui->checkAuto->isChecked()){

    }else{


       // Rect templateRect((640-80)/2,0.3*(480-80)/3,80,80);
        // Mat templateImage = imread("C:/Users/Mon PC/Desktop/Tap_The_Rainbow/Tap_The_Rainbow/Snap2.JPG");
        if (cam->isOpened()) {
            Mat image;
            Mat resultImage;
            int width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
            int height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);
            if (cam->read(image)){ //image est notre image de calibrage.
                //Il faut appliquer des traitements pour trouver le point
                //qui va permettre de découper notre image en 4 imagettes.
                   // namedWindow("Image de calibrage",1);
                   // imshow("Image de calibrage", image);
                    // Motif que l'on recherche
               // QString CurrentDir = QDir::currentPath();
               // qDebug()<<"Chemin : "<<CurrentDir;
                    Mat templateImage = imread("./debug/imagesSrc/TapTheRainBOOOOOOOOW.png");  //QPixmap
                    int result_cols =  image.cols - templateImage.cols + 1;
                    int result_rows = image.rows - templateImage.rows + 1;
                    resultImage.create( result_cols, result_rows, CV_32FC1 );
                   // Rect resultRect;
                    // Do the Matching between the frame and the templateImage
                    matchTemplate( image, templateImage, resultImage, TM_CCORR_NORMED );
                    // Localize the best match with minMaxLoc
                    double minVal; double maxVal; Point minLoc; Point maxLoc;
                    minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

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
                  // QString instrument= detectionInstrument(Img4,maxLoc);
                  // qDebug()<<instrument;
             }
        }
        else {
            ui->camInfo->setText("Error capturing the frame");
        }
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
    qDebug()<<"clic";

     int lPositionX = event->x();
     int lPositionY = event->y();

     PositionX=&lPositionX;
     PositionY=&lPositionY;
     qDebug()<<"x1: "<<*PositionX<<" et y1: "<<*PositionY;
     //qDebug() <<"Position globale :" << event->globalX() << "," << event->globalY() << ")\n";
    ok=false;
}

void MainWindow::on_checkAuto_clicked()
{

        if (!ok){

            if (cam->isOpened()) {
                Mat image;

                int width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
                int height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);
                if (cam->read(image)){ //image est notre image de calibrage.
                Point maxLoc;
                maxLoc.x = *PositionX;
                maxLoc.y = *PositionY;


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
                      // QString instrument= detectionInstrument(Img4,maxLoc);
                      // qDebug()<<instrument;
                 }
            }
            else {
                ui->camInfo->setText("Error capturing the frame");
            }


        }
       /* int lPositionX = event->x();
        int lPositionY = event->y();
         qDebug()<<"x: "<<lPositionX<<" et y: "<<lPositionY;*/

}
