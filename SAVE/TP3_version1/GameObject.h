#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "GameComponent.h"
#include "Transform.h"

class GameObject{

private :
    std::vector<GameObject> children;
    std::vector<GameComponent> components;
    Transform transform;

public :
    GameObject() = default;
    void input();
    void update();
    void render();
    void addChild(GameObject child){children.push_back(child);}
    void addComponent(GameComponent component){components.push_back(component);}
    Transform getTransform(){return transform;}

};




#endif // GAMEOBJECT_H
