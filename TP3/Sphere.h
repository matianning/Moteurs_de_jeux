#ifndef SPHERE_H
#define SPHERE_H

#include "GameObject.h"

class Sphere : public GameObject{
public :
    Sphere();
    ~Sphere(){
         arrayBuf.destroy();
         indexBuf.destroy();
    }
    void init() override;
    void update() override;
    void render(QOpenGLShaderProgram *program) override;
    int size_vertices = 0; //size_vertices
    int size_indices = 0; //size_index

private :

    QVector3D * t_vertices;
    GLushort * t_indices;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};









#endif // SPHERE_H
