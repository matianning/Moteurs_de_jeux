#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "gameObject.h"

class PhysicsEngine{

public :
    PhysicsEngine() = default;

    void addObject(const GameObject & obj){ physicsObjects.push_back(obj);}
    void handleCollisions();


private :

    std::vector<GameObject> physicsObjects;










};




#endif // PHYSICSENGINE_H
