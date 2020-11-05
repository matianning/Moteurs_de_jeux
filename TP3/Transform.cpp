#include "Transform.h"

Transform::Transform(const QVector3D & pos  , const QVector3D & rot , const QVector3D & scal)
    : position(pos), rotation(rot), scale(scal)
{
    OriginalMatrix.setToIdentity();
    AppliedMatrix.setToIdentity();
}

QMatrix4x4 Transform::apply(){

    AppliedMatrix.translate(position);
    AppliedMatrix.rotate(rotation.x(), QVector3D(1.0f, 0.0f, 0.0f));
    AppliedMatrix.rotate(rotation.y(), QVector3D(0.0f, 1.0f, 0.0f));
    AppliedMatrix.rotate(rotation.z(), QVector3D(0.0f, 0.0f, 1.0f));
    AppliedMatrix.scale(scale);

    return AppliedMatrix;
}
