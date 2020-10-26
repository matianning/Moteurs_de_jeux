#include "GameObject.h"


void GameObject::input(){
/*
    for(GameComponent component : components){
        component.input();
    }
*/
    for(GameObject child : children){
        child.input();
    }
}

void GameObject::update(){
  /*
    for(GameComponent component : components){
        component.update();
    }
*/
    for(GameObject child : children){
        child.update();
    }
}

void GameObject::render(){
  /*
    for(GameComponent component : components){
        component.render();
    }
*/
    for(GameObject child : children){
        child.render();
    }
}
