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
    void decoupageImage();
private slots:
    void update();
    void on_pushButton_clicked();
    void on_checkAuto_clicked();
    void on_imgCalib_clicked();

    void on_butonMulti_clicked();

    void on_butonJoueur_clicked();

    void on_checkBox_clicked();

    void on_instrument1_clicked();

private:
    Ui::MainWindow *ui;
    VideoCapture *cam;
    QTimer *timer = new QTimer(this);
    int *PositionX=0;
    int *PositionY=0;

    bool manu=false;  // on active le calibrage manu
    bool multi = false; //multijoueurs ou mode solo
    bool imagette=false; // on souhaite voir les imagettes
    bool key=false; //valide au moins un clic de l'utilisateur
    //instruments activés
    bool instrument1 = false;
    bool instrument2 = false;
    bool instrument3 = false;
    bool instrument4 = false;
    Traitement_images TI;
    bool calibrationOk;
    Mat calibrationImg;
    Mat calibration;
    Mat capture;
    Mat image;
    Mat Img1;
    Mat Img2;
    Mat Img3;
    Mat Img4;
    int width;
    int height;
    Point centreImg;
};

#endif // MAINWINDOW_H
