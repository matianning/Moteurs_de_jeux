#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include "IntersectData.h"
#include <QVector3D>


class BoundingSphere{

public :
    BoundingSphere(QVector3D c, float r) : center(c), radius(r){}

    IntersectData IntersectBoundingSphere( const BoundingSphere & other);

    QVector3D   getCenter() const    {return center;}
    float       getRadius() const    {return radius;}

private :
    QVector3D   center;
    float       radius;
};


#endif // BOUNDINGSPHERE_H
