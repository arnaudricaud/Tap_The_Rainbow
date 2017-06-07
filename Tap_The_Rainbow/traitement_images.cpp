#include "traitement_images.h"
#include <QElapsedTimer>

Traitement_images::Traitement_images()
{
}

void Traitement_images::reconstruction(Mat calibration, Mat capture){
    //DEBUT
    Mat origine = calibration.clone();
    Mat frame = capture.clone();

    //MATRICE SOUSTRACTION ET NIVEAU DE GRIS
    Mat soustraction;
    Mat imgNDG;

    //ELEMENT STRUCTURANT
    Mat kernel1 = getStructuringElement(MORPH_ELLIPSE,Size(1,1));


   //detection de contour
    cvtColor(origine,imgNDG,CV_RGB2GRAY);
    Canny(imgNDG,imgNDG,100,100);
    morphologyEx(imgNDG,imgNDG,MORPH_DILATE,kernel1); //Dilatation
    floodFill(imgNDG,cv::Point(50,10),Scalar(255)); //Remplissage
    bitwise_not(imgNDG,imgNDG); //Complémentaire

    // Récupération des RONDS uniquement! (Evite légèrement le pb des ombres)
    for (int i=0;i<imgNDG.cols;i++){
        for (int j=0;j<imgNDG.rows;j++){
            if((int)imgNDG.at<uchar>(j,i)==0){
                origine.at<Vec3b>(j,i)[0]=0;
                origine.at<Vec3b>(j,i)[1]=0;
                origine.at<Vec3b>(j,i)[2]=0;
            }
        }
    }


    // soustraction
    soustraction=origine-frame; //SOUSTRACTION => Doigt sur les ronds

    // binarisation
    cvtColor(soustraction,soustraction,CV_RGB2GRAY);
    threshold(soustraction,soustraction,35,255,CV_THRESH_BINARY); //35

    //PREND 66ms
    // detection de couleur et son
    detectCouleur(origine, soustraction);

}

void Traitement_images::detectCouleur(Mat image, Mat mask){

    //DEBUT TIMER

    float compteur=0;
    float valeur=0;
    float moyenne=0;
    cvtColor(image,image,CV_BGR2HSV);
    for (int i=0;i<mask.cols;i++){
        for (int j=0;j<mask.rows;j++){
            if((int)mask.at<uchar>(j,i)!=0){
                compteur++;
                valeur=valeur+image.at<Vec3b>(j,i)[0];//Valeurs autours de 50/60
            }
        }
    }
    //PREND 24 ms


    //DEBUT TIMER
    // determination de la couleur
    moyenne=valeur/compteur;
    //imshow("resultat finale",image);
    //QElapsedTimer timer;
    //timer.start();
    float teinte=0;
    teinte=moyenne*360/180;
    if (compteur>1000){
        if(teinte>15 && teinte<45){
            qDebug()<<"orange";
        }else if(teinte>=45 && teinte<75){
            qDebug()<<"jaune";
            note.playNote(2);
        }else if(teinte>=75 && teinte<105){
            qDebug()<<"jaune-vert";
        }else if(teinte>=105 && teinte<135){
            qDebug()<<"vert";
            note.playNote(4);
        }else if(teinte>=135 && teinte<165){
            qDebug()<<"bleu-vert";
        }else if(teinte>=165 && teinte<195){
            qDebug()<<"cyan";
        }else if(teinte>=195 && teinte<225){
            qDebug()<<"bleu moyen";
            note.playNote(1);
        }else if(teinte>=225 && teinte<255){
            qDebug()<<"bleu ";
        }else if(teinte>=255 && teinte<285){
            qDebug()<<"violet";
        }else if(teinte>=285 && teinte<315){
            qDebug()<<"magenta";
            note.playNote(5);
        }else if(teinte>=315 && teinte<345){
            qDebug()<<"rose";
        }else{
            qDebug()<<"rouge";
            note.playNote(3);
        }
    }

    //PRENDS 13 ms
}
