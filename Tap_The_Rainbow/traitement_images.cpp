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

void Traitement_images::detectCouleur(Mat image){
    float compteur=0;
    float valeur=0;
    float moyenne=0;
    cvtColor(image,image,CV_BGR2HSV);
    for (int i=0;i<image.cols;i++){
        for (int j=0;j<image.rows;j++){
            if((int)image.at<Vec3b>(j,i)[0]!=0){
                compteur++;
                valeur=valeur+image.at<Vec3b>(j,i)[0];
            }
        }
    }


// determination de la couleur
    moyenne=valeur/compteur;
    qDebug()<<compteur;
    qDebug()<<moyenne;
    imshow("resultat finale",image);

    float teinte=0;
    teinte=moyenne*360/180;

    if(teinte>15 && teinte<45){
        qDebug()<<"orange";
    }else if(teinte>=45 && teinte<75){
        qDebug()<<"jaune";
    }else if(teinte>=75 && teinte<105){
        qDebug()<<"jaune-vert";
    }else if(teinte>=105 && teinte<135){
        qDebug()<<"vert";
        note.playNote(1);
    }else if(teinte>=135 && teinte<165){
        qDebug()<<"bleu-vert";
    }else if(teinte>=165 && teinte<195){
        qDebug()<<"cyan";
    }else if(teinte>=195 && teinte<225){
        qDebug()<<"bleu moyen";
    }else if(teinte>=225 && teinte<255){
        qDebug()<<"bleu ";
    }else if(teinte>=255 && teinte<285){
        qDebug()<<"violet";
    }else if(teinte>=285 && teinte<315){
        qDebug()<<"magenta";
    }else if(teinte>=315 && teinte<345){
        qDebug()<<"rose";
    }else{qDebug()<<"rouge";
    }

}
