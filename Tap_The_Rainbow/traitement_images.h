#ifndef TRAITEMENT_IMAGES_H
#define TRAITEMENT_IMAGES_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <QDebug>

using namespace cv;


class Traitement_images
{
public:
    Traitement_images();
    Mat reconstruction(Mat calibration,Mat capture);
};

#endif // TRAITEMENT_IMAGES_H
