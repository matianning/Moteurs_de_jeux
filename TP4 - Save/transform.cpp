#include "transform.h"
#include <QVector4D>


Transform::Transform()
{
    translation=QVector3D(0,0,0);
    scale=QVector3D(1,1,1);
    rotation=QQuaternion(0,0,0,0);
    transformMat.setToIdentity();
}

Transform::Transform(QVector3D translate, QVector3D s, QQuaternion r)
{
    translation=translate;
    rotation=r;
    scale=s;
    QMatrix3x3 tmp = r.toRotationMatrix();
    transformMat = QMatrix4x4(tmp(0,0)*scale.x(),tmp(0,1),tmp(0,2),translation.x(),
                              tmp(1,0),tmp(1,1)*scale.y(),tmp(1,2),translation.y(),
                              tmp(2,0),tmp(2,1),tmp(2,2)*scale.z(),translation.z(),
                              0,0,0,1);
}

QVector3D Transform::applyToPoint(QVector3D p)
{
    QVector4D tmp = QVector4D(p.x(),p.y(),p.z(),1) * transformMat;
    return QVector3D(tmp.x(),tmp.y(),tmp.z());
}

QVector3D Transform::applyToVector(QVector3D v)
{
    QVector4D tmp = QVector4D(v.x(),v.y(),v.z(),1) * transformMat;
    return QVector3D(tmp.x(),tmp.y(),tmp.z());
}

QVector3D Transform::applyToVersor(QVector3D v)
{
    QVector4D tmp = QVector4D(v.x(),v.y(),v.z(),1) * transformMat;
    return QVector3D(tmp.x(),tmp.y(),tmp.z());
}

Transform Transform::combineWith(Transform &t)
{
    //?
    return t;
}

Transform Transform::inverse()
{
    //?
    return Transform();
}

Transform Transform::interpolateWith(Transform &t, float k)
{
    Transform res;
    res.scale = scale * k + t.scale*(1-k);
    res.translation = this->translation*k + t.translation*(k-1);
    res.rotation = this->rotation*k + t.rotation*(k-1);
    res.rotation.normalize();
    return res;
}

QQuaternion Transform::getRotation()
{
    return this->rotation;
}

QVector3D Transform::getTranslation()
{
    return this->translation;
}

QVector3D Transform::getScaling()
{
    return this->scale;
}

void Transform::setRotation(QQuaternion r)
{
    this->rotation=r;
    QMatrix3x3 tmp = r.toRotationMatrix();
    transformMat = QMatrix4x4(tmp(0,0)*scale.x(),tmp(0,1),tmp(0,2),translation.x(),
                              tmp(1,0),tmp(1,1)*scale.y(),tmp(1,2),translation.y(),
                              tmp(2,0),tmp(2,1),tmp(2,2)*scale.z(),translation.z(),
                              0,0,0,1);
}

void Transform::setTranslation(QVector3D t)
{
    this->translation=t;
    QMatrix3x3 tmp = rotation.toRotationMatrix();
    transformMat = QMatrix4x4(tmp(0,0)*scale.x(),tmp(0,1),tmp(0,2),translation.x(),
                              tmp(1,0),tmp(1,1)*scale.y(),tmp(1,2),translation.y(),
                              tmp(2,0),tmp(2,1),tmp(2,2)*scale.z(),translation.z(),
                              0,0,0,1);
}

void Transform::setScaling(QVector3D s)
{
    this->scale=s;
    QMatrix3x3 tmp = rotation.toRotationMatrix();
    transformMat = QMatrix4x4(tmp(0,0)*scale.x(),tmp(0,1),tmp(0,2),translation.x(),
                              tmp(1,0),tmp(1,1)*scale.y(),tmp(1,2),translation.y(),
                              tmp(2,0),tmp(2,1),tmp(2,2)*scale.z(),translation.z(),
                              0,0,0,1);
}
