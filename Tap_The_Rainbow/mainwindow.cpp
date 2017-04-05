#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>

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
        }
        else {
            ui->camInfo->setText("Error capturing the frame");
        }

    }
}
