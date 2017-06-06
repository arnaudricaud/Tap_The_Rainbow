#ifndef TRAITEMENT_IMAGES_H
#define TRAITEMENT_IMAGES_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "playsound.h"
#include <QDebug>

using namespace cv;


class Traitement_images
{
public:
    Traitement_images();
    void reconstruction(Mat calibration,Mat capture);
    void detectCouleur(Mat image, Mat mask);
private:
    PlaySound note;
};

#endif // TRAITEMENT_IMAGES_H
