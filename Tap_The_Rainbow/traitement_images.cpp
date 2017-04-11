#include "traitement_images.h"

Traitement_images::Traitement_images()
{
}

Mat Traitement_images::reconstruction(Mat calibration, Mat capture){
    Mat image3;
    Mat image4;
    //detection de contour
    cvtColor(calibration,image4,CV_RGB2GRAY);
    Canny(image4,image4,100,100);
    Mat kernel1 = getStructuringElement(MORPH_ELLIPSE,Size(5,5));
    morphologyEx(image4,image4,MORPH_DILATE,kernel1);
    floodFill(image4,cv::Point(500,500),Scalar(255));
    floodFill(image4,cv::Point(0,0),Scalar(255));
    floodFill(image4,cv::Point(1590,0),Scalar(255));
    bitwise_not(image4,image4);

    for (int i=0;i<image4.cols;i++){
        for (int j=0;j<image4.rows;j++){
            if((int)image4.at<uchar>(j,i)==0){
                calibration.at<Vec3b>(j,i)[0]=0;
                calibration.at<Vec3b>(j,i)[1]=0;
                calibration.at<Vec3b>(j,i)[2]=0;
            }
        }
    }

    // soustraction
    image3=calibration-capture;
    // ouverture
    Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(50,50));
    morphologyEx(image3,image3,MORPH_OPEN,kernel);
    // binarisation
    cvtColor(image3,image3,CV_RGB2GRAY);
    equalizeHist(image3,image3);
    threshold(image3,image3,7,255,CV_THRESH_BINARY);
    // reconstruction
    for (int i=0;i<image3.cols;i++){
        for (int j=0;j<image3.rows;j++){
            if((int)image3.at<uchar>(j,i)==0){
                calibration.at<Vec3b>(j,i)[0]=0;
                calibration.at<Vec3b>(j,i)[1]=0;
                calibration.at<Vec3b>(j,i)[2]=0;
            }
        }
    }
    return calibration;
}
