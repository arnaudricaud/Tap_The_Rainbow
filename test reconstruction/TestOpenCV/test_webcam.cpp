#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
        return -1;
    }

    Mat frame_1;
    Mat frame_2;
    Mat frame_3;
    Mat frame_4;
    Mat frame_5;
    Mat frame_fin;
    namedWindow("WebCam",1);
    for(;;)
    {
        cap >> frame_1; // get a new frame from camera
        flip(frame_1,frame_1,1); // effet miroir
        cvtColor(frame_1,frame_1,CV_RGB2GRAY);
        threshold(frame_1,frame_2,50,100,1);
        imshow("WebCam", frame_2);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
