#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "GameComponent.h"
#include "Transform.h"
#include "BasicIO.h"
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>


class GameObject{

private :
    std::vector<GameObject> children;
    std::vector<GameComponent> components;
    Transform transform;

public :
    GameObject() = default;
    void init(std::string filename);

    void input();   //Initilisation de l'objet
    void update();  //mettre à jour (eg transformation) de l'objet
    void render();  //dessiner (à nouveau)

    void addChild(GameObject child){children.push_back(child);}
    void addComponent(GameComponent component){components.push_back(component);}
    Transform getTransform(){return transform;}

};




#endif // GAMEOBJECT_H
