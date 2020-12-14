#include "physicsEngine.h"


//Check pour chaque objet s'ils ont une intersection
void PhysicsEngine::handleCollisions(){
    for(unsigned int i = 0; i < physicsObjects.size(); i++){
        for(unsigned int j = 0; j < physicsObjects.size(); j++) {
            //physicsObjects[i].getBoundingBox()
            //physicsObjects[j].getBoudningBox();

            /*
            if(intersectData){
                physicsObjects[i].setVelocity(physicsObjects[i].getVelocity() * -1);
                physicsObjects[j].setVelocity(physicsObjects[j].getVelocity() * -1);
            }
            */

        }
    }
}
