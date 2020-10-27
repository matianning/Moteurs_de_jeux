#include "GameObject.h"

void GameObject::init(std::string filename){
    std::vector<QVector3D> vertices;
    std::vector<std::vector<GLushort>> indices;
    OBJIO::open(filename,vertices,indices);

    int size_vertices = vertices.size();
    int size_indices = indices.size() * 3;
    QVector3D * t_vertices = new QVector3D[size_vertices];
    GLushort * t_indices = new GLushort[size_indices];
}

void GameObject::input(){

    for(GameComponent component : components){
        component.input();
    }

    for(GameObject child : children){
        child.input();
    }
}

void GameObject::update(){

    for(GameComponent component : components){
        component.update();
    }

    for(GameObject child : children){
        child.update();
    }
}

void GameObject::render(){

    for(GameComponent component : components){
        component.render();
    }

    for(GameObject child : children){
        child.render();
    }
}
