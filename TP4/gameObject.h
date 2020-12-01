#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "transform.h"
#include <vector>


enum objectType{SPHERE, PLANE};
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

class GameObject : protected QOpenGLFunctions_3_1{
public :
    GameObject() = default;
    GameObject(objectType type);
    GameObject(const GameObject & obj);
    virtual ~GameObject();

    //*********** Basic Function *************
    void init();
    void update();
    void render(QOpenGLShaderProgram *program, QMatrix4x4 projection);


    //************Graph related*****************
    void addChild(GameObject child){children.push_back(child);}
    int sizeChildren(){return children.size();}
    Transform getTransform(){return transform;}



    //*************Render related ***************
    void drawPlane(QOpenGLShaderProgram *program);
    void drawObjGeometry(QOpenGLShaderProgram *program, QMatrix4x4 projection);
    bool polygone_line = false;
    static const int _x = 16;
    static const int _y = 16;
    static const int taille_index = (_x -1) * ((2 * _y) + 2) - 2;
    float ratio = 10.0f;    //pour zoomer "ratio" fois plus grand

    //*************Collision related**************
    void calculateCenter(QMatrix4x4 projection);
    QVector3D getCenter(){return center;}
    void move(float x, float y, float z);
    float getHauteur(QVector3D p);  //Retourner l'hauteur correspondante selon un point

private :
    void initPlaneGeometry();
    void initObjGeometry(std::string filename);

    int IndexSize = 0;
    std::vector<QVector3D> vertices;
    std::vector<std::vector<GLushort>> indices;

    std::vector<QVector3D> vertices_plane;


    //Buffer
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;


    float offset_sphere = 2.0f;
    objectType type;
    QVector3D center;       //Barycentre du objet
    std::vector<QVector3D> centers; //Barycentre des triangles
    Transform transform;
    std::vector<GameObject> children;
};


#endif // GAMEOBJECT_H
