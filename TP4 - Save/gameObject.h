#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "transform.h"
#include <vector>


enum objectType{SPHERE, PLANE};

class GameObject{
public :
    GameObject() = default;
    GameObject(objectType type);

    void addChild(GameObject child){children.push_back(child);}
    int sizeChildren(){return children.size();}
    Transform getTransform(){return transform;}

    void init();
    void render();


private :
    objectType type;
    Transform transform;
    std::vector<GameObject> children;
};


#endif // GAMEOBJECT_H
