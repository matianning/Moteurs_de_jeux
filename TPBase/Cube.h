#ifndef CUBE_H
#define CUBE_H

#include "GameObject.h"

class Cube : GameObject{
public :
    Cube();
    Cube(float x, float y, float z);

    float getLongeur(){return this->longeur;}
    float getLargeur(){return this->largeur;}
    float gethauteur(){return this->hauteur;}
    void setLongueur(float l){this->longeur = l;}
    void setLargeur(float l){this->largeur = l;}
    void setHauteur(float h){this->hauteur = h;}

private :
    float longeur;
    float largeur;
    float hauteur;
};











#endif // CUBE_H
