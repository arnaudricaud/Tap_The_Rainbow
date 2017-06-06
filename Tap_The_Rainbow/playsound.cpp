#include "playsound.h"

PlaySound::PlaySound()
{

}

void PlaySound::playNote(int couleurs){
    switch(couleurs){
    case 1 : QSound::play(":/sons/Ressource/MLDClapLess.wav");
        break;
    case 2 : QSound::play(":/sons/Ressource/MLDSnareDeeper.wav");
        break;
    case 3 : QSound::play(":/sons/Ressource/MLDKickKnockLong.wav");
        break;
    case 4 : QSound::play(":/sons/Ressource/MLDCowbell.wav");
        break;
    case 5 : QSound::play(":/sons/Ressource/MLDHat2.wav");
        break;
    case 6 : QSound::play(":/sons/Ressource/MLDClapLess.wav");
        break;
    case 7 : QSound::play(":/sons/Ressource/MLDSnareDeeper.wav");
        break;
    case 8 : QSound::play(":/sons/Ressource/MLDKickKnockLong.wav");
        break;
    case 9 : QSound::play(":/sons/Ressource/MLDCowbell.wav");
        break;
    case 10 : QSound::play(":/sons/Ressource/MLDHat2.wav");
        break;
    case 11 : QSound::play(":/sons/Ressource/MLDKickKnockLong.wav");
        break;
    case 12 : QSound::play(":/sons/Ressource/MLDCowbell.wav");
        break;
    case 13 : QSound::play(":/sons/Ressource/MLDHat2.wav");
        break;
    default : qDebug()<<"Something went wrong";
        break;
    }
}
