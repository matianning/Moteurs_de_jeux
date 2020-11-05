#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>


class Transform {

public :
    Transform(const QVector3D & position = QVector3D(0.0f,0.0f,0.0f),
              const QVector3D & rotation = QVector3D(0.0f,0.0f,0.0f),
              const QVector3D & scale = QVector3D(1.0f,1.0f,1.0f));

    QMatrix4x4 apply();
    QVector3D & getPosition() { return position; }
    QVector3D & getRotation() { return rotation; }
    QVector3D & getScale() { return scale; }
    QMatrix4x4 & getOriginalMatrix() { return OriginalMatrix; }
    QMatrix4x4 & getAppliedMatrix(){return AppliedMatrix;}


    void setPosition(const QVector3D & pos) { position = pos; }
    void setRotation(const QVector3D & rot) { rotation = rot; }
    void setScale(const QVector3D & scal) { scale = scal; }

private :
    QVector3D position;
    QVector3D rotation;
    QVector3D scale;
    QMatrix4x4 OriginalMatrix;
    QMatrix4x4 AppliedMatrix;

};





#endif // TRANSFORM_H
