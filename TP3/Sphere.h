#ifndef SPHERE_H
#define SPHERE_H

#include "GameObject.h"

class Sphere : public GameObject{
public :
    Sphere();
    void init(QOpenGLBuffer& arrayBuf, QOpenGLBuffer& indexBuf) override;
    void update() override;
    void render(QOpenGLShaderProgram *program, QOpenGLBuffer& arrayBuf, QOpenGLBuffer& indexBuf) override;
    int size_vertices = 0; //size_vertices
    //    int size_indices = 0; //size_index

private :




    QVector3D * t_vertices;
    GLushort * t_indices;
};









#endif // SPHERE_H
