#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "transform.h"
#include <vector>


enum objectType{SPHERE, PLANE};

class GameObject : protected QOpenGLFunctions_3_1{
public :
    GameObject() = default;
    GameObject(objectType type);
    GameObject(const GameObject & obj);
    virtual ~GameObject();

    void init();
    void render(QOpenGLShaderProgram *program);

    //************Graph related*****************
    void addChild(GameObject child){children.push_back(child);}
    int sizeChildren(){return children.size();}
    Transform getTransform(){return transform;}



    //*************Render related ***************
    void drawPlane(QOpenGLShaderProgram *program);
    void drawObjGeometry(QOpenGLShaderProgram *program);
    bool polygone_line = false;
    static const int _x = 16;
    static const int _y = 16;
    static const int taille_index = (_x -1) * ((2 * _y) + 2) - 2;
    float ratio = 10.0f;    //pour zoomer "ratio" fois plus grand


private :
    void initPlaneGeometry();
    void initObjGeometry(std::string filename);

    int IndexSize = 0;

    //Buffer
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;



    objectType type;
    Transform transform;
    std::vector<GameObject> children;
};


#endif // GAMEOBJECT_H
