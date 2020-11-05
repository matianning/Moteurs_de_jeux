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
    std::vector<GameObject *> children;
    std::vector<GameComponent> components;
    Transform transform;

public :

    GameObject() = default;

    virtual void init(QOpenGLBuffer& arrayBuf,
                      QOpenGLBuffer& indexBuf);   //Initilisation de l'objet
    virtual void update(QOpenGLBuffer& arrayBuf,
                        QOpenGLBuffer& indexBuf);  //mettre à jour (eg transformation) de l'objet
    virtual void render(QOpenGLShaderProgram *program,
                        QOpenGLBuffer & arrayBuf,
                        QOpenGLBuffer & indexBuf);  //dessiner (à nouveau)

    void translate(const QVector3D & t);
    void rotate(const QVector3D & r);
    void scale(const QVector3D & scale);

    void addChild(GameObject * child){children.push_back(child);}
    void addComponent(GameComponent component){components.push_back(component);}
    Transform getTransform(){return transform;}

    int size_indices = 0;
};




#endif // GAMEOBJECT_H
