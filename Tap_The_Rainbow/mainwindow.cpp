#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(;;)
    {

       imshow("Frame" ,cam->getFrame());
       //ui->camFrame->setPixmap(QPixmap(img);
       if(waitKey(30) >= 0) break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

