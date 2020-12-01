#include "BoundingSphere.h"

IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere & other){
    float radiusDistance = radius + other.getRadius();
    float centerDistance = center.distanceToPoint(other.getCenter());

    float distance = centerDistance - radiusDistance;
    return IntersectData(distance < 0, distance);
}
