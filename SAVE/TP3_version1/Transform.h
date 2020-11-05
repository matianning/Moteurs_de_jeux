#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>


class Transform {

public :
    Transform()=default;
    QMatrix4x4 m;
    QVector4D apply(QVector4D p);


};









#endif // TRANSFORM_H
