#include "gameObject.h"
#include "BasicIO.h"
#include <QImage>
#include <iostream>

float distance(QVector3D p1, QVector3D p2){
    return sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y()) + (p1.z()-p2.z())*(p1.z()-p2.z()));
}

GameObject::GameObject(objectType t)
    :indexBuf(QOpenGLBuffer::IndexBuffer),type(t),center(0.0,0.0,0.0)
{
    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();

    init();
}

GameObject::GameObject(const GameObject & obj)
    :indexBuf(QOpenGLBuffer::IndexBuffer),type(obj.type){

    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();

    init();
}

GameObject::~GameObject()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}



void GameObject::init(){
    switch (type) {
    case objectType::SPHERE :
        //************Gestionnaire de détail*****************
        distance_objet_camera = distance(cameraPosition, center);
        if(distance_objet_camera < seuil1){ //Très proche
            initObjGeometry("sphere.obj");
        }
        else if (distance_objet_camera >= seuil1 && distance_objet_camera < seuil2){ //Distance
            initSimplifiedObjGeometry("sphere.obj");
        }
        else{   //loin de camera
            initMinimalistObjGeometry("sphere.obj");
        }
        break;
    case objectType::PLANE :
        initPlaneGeometry();
        break;
    default : break;
    }
}


void GameObject::render(QOpenGLShaderProgram *program, QMatrix4x4 projection){
    /*
    for(GameObject child : children){
        child.render();
    }
    */

    switch (type) {
    case objectType::SPHERE :
        drawObjGeometry(program, projection);
        break;

    case objectType::PLANE :
        drawPlane(program);
        break;
    default : break;
    }
}

void GameObject::update(){
    QVector3D * t_vertices = new QVector3D[vertices.size()];
    for(size_t i = 0; i < vertices.size(); i++){
        t_vertices[i] = vertices[i];
    }
    arrayBuf.bind();
    arrayBuf.allocate(t_vertices, vertices.size() * sizeof(QVector3D));
}

//********************************************************************
//*****************************OBJ**********************************
//********************************************************************




void GameObject::initObjGeometry(std::string filename)
{
   // std::vector<QVector3D> vertices;
   // std::vector<std::vector<GLushort>> indices;
    OBJIO::open(filename,vertices,indices);

    int size_vertices = vertices.size();
    int size_indices = indices.size() * 3;

    QVector3D * t_vertices = new QVector3D[vertices.size()];
    GLushort * t_indices = new GLushort[indices.size() * 3];

    for(size_t i = 0; i < vertices.size(); i++){
        t_vertices[i] = vertices[i];
    }
    for(size_t i = 0; i < indices.size(); i++){
        for(size_t j = 0; j < indices[i].size(); j++){
            t_indices[i * 3 + j] = indices[i][j];
        }
    }

    this->IndexSize = size_indices; // sauvegarder size pour render
    arrayBuf.bind();
    arrayBuf.allocate(t_vertices, size_vertices * sizeof(QVector3D));

    indexBuf.bind();
    indexBuf.allocate(t_indices, size_indices * sizeof(GLushort));


}

void GameObject::calculateCenter(QMatrix4x4 projection){
    QVector3D barycentre(0.0,0.0,0.0);
    for(size_t i = 0; i < vertices.size(); i++){
        barycentre += vertices[i];
    }
    barycentre/=vertices.size();


    center = barycentre;

    QMatrix4x4 matrix;
    matrix.setToIdentity();

    matrix.rotate(90,QVector3D(1.0,0.0,0.0));
    matrix.translate(5.0,4.0,-1.0);

    matrix.translate(transform.getTranslation());   //Pour le movement de sphère
    matrix.rotate(transform.getRotation());
    matrix.scale(transform.getScaling());

    QMatrix4x4 tmp = projection * matrix;
    QVector4D tmp_c = QVector4D(center.x(),center.y(),center.z(),1.0);
    QVector4D res =  tmp * tmp_c;
    center = res.toVector3D();
    //std::cout<<center.x()<<","<<center.y()<<","<<center.z()<<std::endl;
}



float GameObject::getHauteur(QVector3D p){
    float hauteur(0.0);

   // std::cout<<"p : "<<p.x()<<","<<p.y()<<","<<p.z()<<std::endl;

    QVector3D point(p.x(),p.y(),0.0);

    float min(1000.0f); int index(0);
    for(size_t i = 0; i < vertices_plane.size(); i++){
        //std::cout<<i<<" : "<<vertices_plane[i].x()<<","<<vertices_plane[i].y()<<","<<vertices_plane[i].z()<<std::endl;
        QVector3D p_projete(vertices_plane[i].x(), vertices_plane[i].y(), 0.0);
        float dist = distance(point,p_projete);
        if(dist <= min){
            min = dist;
            index = i;
        }
    }
    hauteur = vertices_plane[index].z();
    //std::cout<<"i : "<<index<<" Hauteur : "<<hauteur<<std::endl;
    return hauteur-offset_sphere;
}


void GameObject::move(float x, float y, float z){
    transform.setTranslation(QVector3D(x,y,z));
}

void GameObject::drawObjGeometry(QOpenGLShaderProgram *program, QMatrix4x4 projection)
{

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    quintptr offset = 0;
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));
    if(this->polygone_line) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    //*********Transformation appliquée à la sphère************
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.rotate(90,QVector3D(1.0,0.0,0.0));
    matrix.translate(5.0,4.0,-1.0);

    matrix.translate(transform.getTranslation());   //Pour le movement de sphère
    matrix.rotate(transform.getRotation());
    matrix.scale(transform.getScaling());

    program->setUniformValue("mvp_matrix", projection * matrix);

    glDrawElements(GL_TRIANGLES, this->IndexSize, GL_UNSIGNED_SHORT, 0);
}


//********************************************************************
//*****************************Plane**********************************
//********************************************************************


void GameObject::initPlaneGeometry(){

    const int width = GameObject::_x;
    const int height = GameObject::_y;
    VertexData vertices[width*height];

    //**********Remplissage des vertex***************
    for(int i = 0;i < width; i++){
        for(int j = 0;j < height; j++){
            vertices[i * height + j]={QVector3D((float)i/(float)width * ratio, (float)j/(float)height * ratio,  0.0f),
                                      QVector2D(((float) i/(float) width * ratio),((float)j/(float)height * ratio))};
        }
    }

    const int nb_strip = width - 1;
    const int nb_vertex_par_strip = height * 2;

    GLushort indices[GameObject::taille_index];
    int valeur_dupliquee;       //début et la fin de chaque rang de strip

    //**********Remplissage des indices**************
    for(int i=0;i < nb_strip; i++)
    {
        for(int j = 0;j < nb_vertex_par_strip; j++){
            if(j==0){
                indices[j + i * nb_vertex_par_strip + 2 * i - 1]= width * (j%2) + (j/2) + i * width;
            }
            indices[j + i * nb_vertex_par_strip + 2 * i]= width * (j%2) + (j/2) + i * width;
            valeur_dupliquee = width * (j%2) + (j/2) + i * width;

        }
        if(i!= nb_strip-1){
            indices[nb_vertex_par_strip-1 + i * nb_vertex_par_strip + 2 * i + 1] = valeur_dupliquee;
         }
    }

    //Modification de l'altitude des sommets
    QImage heightmap = QImage(QString(":/heightmap-16x16.png"));

    if(!heightmap.isNull()){
        for(int i = 0;i < width; i++){
            for(int j = 0;j < height; j++){
                //Modification d'altutude par un heightmap
                vertices[i * height + j].position[2]=(float)qGray(heightmap.pixel( i, j))/255 * ratio;
                //std::cout<<vertices[i * height + j].position[2]<<std::endl;
            }
        }
    }

    arrayBuf.bind();
    arrayBuf.allocate(vertices, width*height * sizeof(VertexData));

    indexBuf.bind();
    indexBuf.allocate(indices, GameObject::taille_index * sizeof(GLushort));


    //***********Calculer les centres de chaque triangles************
    for(size_t i = 0; i < GameObject::taille_index-2; i++){

        QVector3D c = (vertices[indices[i]].position + vertices[indices[i+1]].position + vertices[indices[i+2]].position) / 3.0;
        //std::cout<<"c: "<<c.x()<<","<<c.y()<<","<<c.z()<<std::endl;
        centers.push_back(c);

    }

    //**********Sauvegarder les vertex du plan pour calcul la hauteur après*********
    vertices_plane.clear();
    for(int i = 0;i < width; i++){
        for(int j = 0;j < height; j++){
            QVector3D tmp(vertices[i * height + j].position[0], vertices[i * height + j].position[1],vertices[i * height + j].position[2]);
            vertices_plane.push_back(tmp);

        }
    }
}

void GameObject::drawPlane(QOpenGLShaderProgram *program){

    arrayBuf.bind();
    indexBuf.bind();
    quintptr offset = 0;

    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLE_STRIP, this->taille_index, GL_UNSIGNED_SHORT, 0);

    if(this->polygone_line) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
