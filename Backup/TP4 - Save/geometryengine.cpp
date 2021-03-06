﻿
#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <vector>
#include <QImage>
#include <iostream>
#include "BasicIO.h"

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer), indexBuf_obj(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();
    arrayBuf_obj.create();
    indexBuf_obj.create();

    initPlaneGeometry();
    initObjGeometry("sphere.obj");
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}

void GeometryEngine::initObjGeometry(std::string filename)
{
    std::vector<QVector3D> vertices;
    std::vector<std::vector<GLushort>> indices;
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
    arrayBuf_obj.bind();
    arrayBuf_obj.allocate(t_vertices, size_vertices * sizeof(QVector3D));

    indexBuf_obj.bind();
    indexBuf_obj.allocate(t_indices, size_indices * sizeof(GLushort));

}

void GeometryEngine::drawObjGeometry(QOpenGLShaderProgram *program)
{

    // Tell OpenGL which VBOs to use
    arrayBuf_obj.bind();
    indexBuf_obj.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    //glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
    glDrawElements(GL_TRIANGLES, this->IndexSize, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::initPlaneGeometry(){

    const int width = GeometryEngine::_x;
    const int height = GeometryEngine::_y;

    VertexData vertices[width*height];

    //**********Remplissage des vertex***************
    for(int i = 0;i < width; i++){
        for(int j = 0;j < height; j++){
            //vertices[i * height + j]={QVector3D((float)i, (float)j,  0.0f),QVector2D((float) i,(float)j)};
            vertices[i * height + j]={QVector3D((float)i/(float)width * ratio, (float)j/(float)height * ratio,  0.0f),
                                      QVector2D(((float) i/(float) width * ratio),((float)j/(float)height * ratio))};
/*
            vertices[i * height + j]={QVector3D((float)i/(float)width, (float)j/(float)height,  0.0f),
                                      QVector2D(((float) i/(float) width),((float)j/(float)height))};
*/
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

    QImage heightmap = QImage(QString(":/heightmap-16x16.png"));

    if(!heightmap.isNull()){
        for(int i = 0;i < width; i++){
            for(int j = 0;j < height; j++){
                //Modification d'altitude par un nombre aléatoire
                //vertices[i * height + j].position[2]=(rand()/(float)RAND_MAX) / 10.0f;

                //Modification d'altutude par un heightmap
                vertices[i * height + j].position[2]=(float)qGray(heightmap.pixel( i, j))/255 * ratio;
                //std::cout<<vertices[i * height + j].position[0]<<","<<vertices[i * height + j].position[1]<<","<<vertices[i * height + j].position[2]<<std::endl;
            }
        }
    }

    arrayBuf.bind();
    arrayBuf.allocate(vertices, width*height * sizeof(VertexData));

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









