#include "playsound.h"

PlaySound::PlaySound()
{

}

void PlaySound::playNote(int couleurs){
    switch(couleurs):
    case 1 : QSound::play("/C:/Users/moulin-d/Downloads/MLDClapLess.wav");
        break;
    case 2 : QSound::play("/C:/Users/moulin-d/Downloads/MLDSnareDeeper.wav");
        break;
    case 3 : QSound::play("/C:/Users/moulin-d/Downloads/MLDKickKnockLong.wav");
        break;
    case 4 : QSound::play("/C:/Users/moulin-d/Downloads/MLDCowbell.wav");
        break;
    case 5 : QSound::play("/C:/Users/moulin-d/Downloads/MLDHat2.wav");
        break;
    defautl : qDebug()<<"Something went wrong";
        break;
}
