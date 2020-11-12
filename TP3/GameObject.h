#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "GameComponent.h"
#include "Transform.h"
#include "BasicIO.h"
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>


class GameObject : protected QOpenGLFunctions_3_1{

protected :
    std::vector<GameObject *> children;
    std::vector<GameComponent> components;
    Transform transform;

public :

    GameObject(){children = std::vector<GameObject*>();
                 components = std::vector<GameComponent>();
                 transform = Transform();}

    virtual void init();   //Initilisation de l'objet
    virtual void update();  //mettre à jour (eg transformation) de l'objet
    virtual void render(QOpenGLShaderProgram *program);  //dessiner (à nouveau)

    void g_translate(const QVector3D & t);
    void g_rotate(const QVector3D & r);
    void g_scale(const QVector3D & scale);

    void addChild(GameObject * child){children.push_back(child);}
    void addComponent(GameComponent component){components.push_back(component);}
    Transform getTransform(){return transform;}

    int indexSize = 0;

};




#endif // GAMEOBJECT_H
