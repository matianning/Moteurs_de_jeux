#include "gameObject.h"

GameObject::GameObject(objectType t)
    :type(t){


}

void GameObject::init(){

/*
    for(GameObject child : children){
        child.init();
    }
*/

    switch (type) {

    case objectType::SPHERE :


        break;

    case objectType::PLANE :


        break;
    default : break;
    }
}


void GameObject::render(){
    /*
    for(GameObject child : children){
        child.render();
    }
    */

    switch (type) {

    case objectType::SPHERE :


        break;

    case objectType::PLANE :


        break;
    default : break;
    }
}
