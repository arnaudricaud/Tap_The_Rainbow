#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

class Camera
{
public:
    VideoCapture cap = VideoCapture(0);
    Camera();
    Mat getFrame();
};

#endif // CAMERA_H
