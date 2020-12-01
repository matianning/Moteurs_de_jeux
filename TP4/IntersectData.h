#ifndef INTERSECTDATA_H
#define INTERSECTDATA_H

/* Class IntersectData stock les informations d'intersection de deux objets */

class IntersectData{
public :

    IntersectData(bool inter, float dist) : intersect(inter), distance(dist){}
    bool getIntersect() const {return intersect;}
    float getDistance() const {return distance;}

private :
        bool intersect;
        float distance;
};




#endif // INTERSECTDATA_H
