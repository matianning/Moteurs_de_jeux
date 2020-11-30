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
/*
    QMatrix3x3 tmp = rotation.toRotationMatrix();
    AppliedMatrix    = QMatrix4x4(tmp(0,0)*scale.x(),tmp(0,1),tmp(0,2),position.x(),
                                  tmp(1,0),tmp(1,1)*scale.y(),tmp(1,2),position.y(),
                                  tmp(2,0),tmp(2,1),tmp(2,2)*scale.z(),position.z(),
                                  0,0,0,1);
*/
    return AppliedMatrix;
}
