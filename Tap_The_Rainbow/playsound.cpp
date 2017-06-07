#include "playsound.h"

PlaySound::PlaySound()
{

}

void PlaySound::playNote(int couleurs){
    QSound *note = 0;
    switch(couleurs){
    case 1 : note = new QSound(":/sons/Ressource/MLDClapLess.wav");
        break;
    case 2 : note = new QSound(":/sons/Ressource/MLDSnareDeeper.wav");
        break;
    case 3 : note = new QSound(":/sons/Ressource/MLDKickKnockLong.wav");
        break;
    case 4 : note = new QSound(":/sons/Ressource/MLDCowbell.wav");
        break;
    case 5 : note = new QSound(":/sons/Ressource/MLDHat2.wav");
        break;
    case 6 : note = new QSound(":/sons/Ressource/MLDClapLess.wav");
        break;
    case 7 : note = new QSound(":/sons/Ressource/MLDSnareDeeper.wav");
        break;
    case 8 : note = new QSound(":/sons/Ressource/MLDKickKnockLong.wav");
        break;
    case 9 : note = new QSound(":/sons/Ressource/MLDCowbell.wav");
        break;
    case 10 : note = new QSound(":/sons/Ressource/MLDHat2.wav");
        break;
    case 11 : note = new QSound(":/sons/Ressource/MLDKickKnockLong.wav");
        break;
    case 12 : note = new QSound(":/sons/Ressource/MLDCowbell.wav");
        break;
    case 13 : note = new QSound(":/sons/Ressource/MLDHat2.wav");
        break;
    default : qDebug()<<"Something went wrong";
        break;
    }
    //note->play();
    while(!note->isFinished()); // attente
    delete note;
}
