#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
using namespace cv;
using namespace std;


#include <iostream>
#include <thread>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>


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
    timer->start(50);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete cam;
}
void MainWindow::update(){

    if (cam->isOpened()) {
        Mat image;
        int width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
        int height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);
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
            qDebug()<<ui->camFrame->pixmap()->size();

             //   cout<<"nb lignes/hauteur :"<<height<<endl; //896
              //  cout<<"nb colonnes/largeur :"<<width<<endl;  //1600
                // HAUT GAUCHE
               Mat Img1 =image;
               Img1 =Img1.colRange(0,width/2);
               Img1=Img1.rowRange(0,height/2);
               cvtColor(image,image,CV_BGR2RGB);
                namedWindow("Haut GAUCHE",1);
                imshow("Haut GAUCHE", Img1);
                // HAUT DROIT
               Mat Img2 =image;
               Img2=Img2.colRange(width/2,width);
               Img2=Img2.rowRange(0,height/2);
              // cvtColor(image,image,CV_BGR2RGB);
                namedWindow("HAUT DROIT",1);
                imshow("HAUT DROIT", Img2);
                // BAS GAUCHE
               Mat Img3 =image;
               Img3=Img3.colRange(0,width/2);
               Img3=Img3.rowRange(height/2,height);
              // cvtColor(image,image,CV_BGR2RGB);
                namedWindow("BAS GAUCHE",1);
                imshow("BAS GAUCHE", Img3);
                // BAS DROIT
               Mat Img4 =image;
               Img4=Img4.colRange(width/2,width);
               Img4=Img4.rowRange(height/2,height);
              // cvtColor(image,image,CV_BGR2RGB);
                namedWindow("BAS DROIT",1);
                imshow("BAS DROIT", Img4);

        }
        else {
            ui->camInfo->setText("Error capturing the frame");
        }

    }
}
