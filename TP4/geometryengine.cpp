
#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <vector>
#include <QImage>
#include <iostream>
#include "BasicIO.h"


void GeometryEngine::createScene(){

    //*************Création de la scène***************

    GameObject plane(objectType::PLANE);
    GameObject sphere(objectType::SPHERE);

    this->gameObjects.clear();
    addObject(plane);
    addObject(sphere);
}



GeometryEngine::GeometryEngine(){
    createScene();
}


GeometryEngine::~GeometryEngine(){
    gameObjects.clear();
    /*
    while(!gameObjects.empty())
       {
           gameObjects.pop_back();
       }
       */
}





