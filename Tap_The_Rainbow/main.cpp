#include "mainwindow.h"
#include <QApplication>
#include "opencv2/opencv.hpp"
#include <iostream>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
