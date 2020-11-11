#include "GameObject.h"


void GameObject::translate(const QVector3D & t){
    this->transform.setPosition(t);
    this->transform.apply();
    for(GameObject * child : children){
        child->transform.setPosition(t);
        child->transform.apply();
    }
}
void GameObject::rotate(const QVector3D & r){
    this->transform.setRotation(r);
    this->transform.apply();
    for(GameObject * child : children){
        child->transform.setRotation(r);
        child->transform.apply();
    }
}
void GameObject::scale(const QVector3D & scale){
    this->transform.setScale(scale);
    this->transform.apply();
    for(GameObject * child : children){
        child->transform.setScale(scale);
        child->transform.apply();
    }
}



void GameObject::init(QOpenGLBuffer arrayBuf, QOpenGLBuffer indexBuf){

    for(GameComponent component : components){
        component.init();
    }

    for(GameObject * child : children){
        child->init(arrayBuf, indexBuf);
    }
}

void GameObject::update(QOpenGLBuffer arrayBuf, QOpenGLBuffer indexBuf){

    for(GameComponent component : components){
        component.update();
    }

    for(GameObject * child : children){
        child->update(arrayBuf,  indexBuf);
    }
}

void GameObject::render(QOpenGLShaderProgram *program, QOpenGLBuffer arrayBuf, QOpenGLBuffer indexBuf){

    for(GameComponent component : components){
        component.render();
    }

    for(GameObject * child : children){
        //this->size_indices += child->size_indices;
        child->render(program, arrayBuf, indexBuf);
    }
}
