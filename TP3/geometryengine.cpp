
#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <vector>
#include <QImage>
#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}



void GeometryEngine::initPlaneGeometry(){

    const int width = GeometryEngine::_x;
    const int height = GeometryEngine::_y;

    VertexData vertices[width*height];

    //**********Remplissage des vertex***************
    for(int i = 0;i < width; i++){
        for(int j = 0;j < height; j++){
            //vertices[i * height + j]={QVector3D((float)i, (float)j,  0.0f),QVector2D((float) i,(float)j)};

            vertices[i * height + j]={QVector3D((float)i/(float)width, (float)j/(float)height,  0.0f),
                                      QVector2D(((float) i/(float) width),((float)j/(float)height))};

        }
    }

    const int nb_strip = width - 1;
    const int nb_vertex_par_strip = height * 2;

    GLushort indices[GeometryEngine::taille_index];
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

    QImage heightmap = QImage(QString(":/heightmap-128x128.png"));

    if(!heightmap.isNull()){
        for(int i = 0;i < width; i++){
            for(int j = 0;j < height; j++){
                //Modification d'altitude par un nombre aléatoire
                //vertices[i * height + j].position[2]=(rand()/(float)RAND_MAX) / 10.0f;

                //Modification d'altutude par un heightmap
                vertices[i * height + j].position[2]=(float)qGray(heightmap.pixel( i, j))/(float)255;
            }
        }
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, width*height * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, GeometryEngine::taille_index * sizeof(GLushort));

}

void GeometryEngine::drawPlane(QOpenGLShaderProgram *program){

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, this->taille_index, GL_UNSIGNED_SHORT, 0);

    if(this->polygone_line) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}











/*
void GeometryEngine::initPlaneGeometry(){

    const int width = GeometryEngine::_x;
    const int height = GeometryEngine::_y;

    QVector<VertexData> vertices;
    QVector<VertexData> * v; v= &vertices;
    //**********Remplissage des vertex***************
    for(int i = 0;i < width; i++){
        for(int j = 0;j < height; j++){
            vertices.push_back({QVector3D((float)i/(float)width, (float)j/(float)height,  0.0f),
                                      QVector2D(((float) i/(float) width),((float)j/(float)height))});
        }
    }

    const int nb_strip = width - 1;
    const int nb_vertex_par_strip = height * 2;

    int nb_indice = (width -1) * ((2 * height) + 2) - 2;
    QVector<int> indices(nb_indice, 0);
    QVector<int> * index = &indices;

    int valeur_dupliquee;       //début et la fin de chaque rang de strip

    //**********Remplissage des indices**************
    for(int i = 0; i < nb_strip; i++)
    {
        for(int j = 0; j < nb_vertex_par_strip; j++){

            if(j==0){
                if(i!=0)
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

    QImage heightmap = QImage(QString(":/heightmap-1024x1024.png"));

    if(!heightmap.isNull()){
        for(int i = 0;i < width; i++){
            for(int j = 0;j < height; j++){
                //Modification d'altitude par un nombre aléatoire
                //vertices[i * height + j].position[2]=(rand()/(float)RAND_MAX) / 10.0f;

                //Modification d'altutude par un heightmap
                vertices[i * height + j].position[2]=(float)qGray(heightmap.pixel(i,j))/(float)255;
            }
        }
    }


    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(v, width*height * sizeof(vertices));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(index, GeometryEngine::taille_index * sizeof(indices));

}

void GeometryEngine::drawPlane(QOpenGLShaderProgram *program){

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, this->taille_index, GL_UNSIGNED_SHORT, 0);

    if(this->polygone_line) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
*/


