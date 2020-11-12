#include "GameObject.h"


void GameObject::g_translate(const QVector3D & t){
    std::cout<<"translate test"<<std::endl;
    this->transform.setPosition(t);

    for(GameObject * child : children){
        if(children.size()!=0){
            child->g_translate(t);
        }
        else{
            child->transform.setPosition(t);
            child->transform.apply();
        }

    }
    this->transform.apply();
}
void GameObject::g_rotate(const QVector3D & r){
    std::cout<<"rotate test"<<std::endl;
    this->transform.setRotation(r);

    std::cout<<"Rotation : "<<r.x()<<","<<r.y()<<","<<r.z()<<std::endl;
    for(GameObject * child : children){
        if(children.size()!=0){
            child->g_rotate(r);
        }
        else{
            child->transform.setRotation(r);
            child->transform.apply();
        }

    }
    this->transform.apply();
}
void GameObject::g_scale(const QVector3D & scale){
    std::cout<<"scale test"<<std::endl;
    this->transform.setScale(scale);
    std::cout<<"children : "<<children.size()<<std::endl;
    for(GameObject * child : children){
        if(child->children.size()!=0){
            child->g_scale(scale);
        }
        else{
            child->transform.setScale(scale);
            child->transform.apply();
        }

    }
    this->transform.apply();
}



void GameObject::init(){

    for(GameComponent component : components){
        component.init();
    }

    for(GameObject * child : children){
        child->init();
    }
}

void GameObject::update(){

    for(GameComponent component : components){
        component.update();
    }

    for(GameObject * child : children){
       // child->transform.apply();
        child->update();
    }
}

void GameObject::render(QOpenGLShaderProgram *program){

    for(GameComponent component : components){
        component.render();
    }

    if(children.size()!=0){
        for(GameObject * child : children){
            //this->size_indices += child->size_indices;
            child->render(program);
        }
    }

}
