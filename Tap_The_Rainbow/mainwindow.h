#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "opencv2/opencv.hpp"
#include <QPixmap>
#include "traitement_images.h"


using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString detectionInstrument(Mat, Point);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void update();
    void on_pushButton_clicked();

    void on_checkAuto_clicked();

    void on_calibrationButton_clicked();

    void on_imgCalib_clicked();

private:
    Ui::MainWindow *ui;
    VideoCapture *cam;
    QTimer *timer = new QTimer(this);
    int *PositionX=0;
    int *PositionY=0;
    bool ok=true;
    Traitement_images TI;
    bool calibrationOk;
    Mat calibrationImg;
    Mat calibration;
    Mat capture;
};

#endif // MAINWINDOW_H
