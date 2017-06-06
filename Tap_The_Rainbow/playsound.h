#ifndef PLAYSOUND_H
#define PLAYSOUND_H
#include <QSound>
#include <QDebug>

//Classe qui permet de jouer de la musique
class PlaySound
{
public:
    PlaySound();
    //Méthode qui joue un son en fonction de l'entier rentrer en paramètre
    void playNote(int couleur);

};

#endif // PLAYSOUND_H
