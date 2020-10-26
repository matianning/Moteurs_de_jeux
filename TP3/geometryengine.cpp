
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

    init();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}



void GeometryEngine::init(){

    std::vector<QVector3D> vertices;
    std::vector<std::vector<GLushort>> indices;


    OBJIO::open("sphere.obj", vertices, indices);




    std::cout<<vertices.size()<<"vertices"<<std::endl;
    std::cout<<indices.size()<<"faces"<<std::endl;
/*
    for(size_t i = 0; i < vertices.size(); i++){
        std::cout<<i<<":"<<vertices[i].x()<<","<<vertices[i].y()<<","<<vertices[i].z()<<std::endl;
    }
*/
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(&vertices[0], vertices.size() * sizeof(QVector3D));

    for(size_t i = 0; i < indices.size(); i++){
        for(size_t j = 0; j < indices[i].size(); j++){
            std::cout<<indices[i][j]<<"-";
        }
        std::cout<<std::endl;
    }

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(&indices[0][0], indices.size() * 3 * sizeof(GLushort));
    this->taille_index = indices.size();

}

void GeometryEngine::draw(QOpenGLShaderProgram *program){

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;


    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 0);
/*
    // Offset for texture coordinate
    offset += sizeof(QVector3D);


    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
*/
    // Draw cube geometry using indices from VBO 1


    glDrawElements(GL_TRIANGLES, 3 * this->taille_index, GL_UNSIGNED_SHORT, 0);


    if(this->polygone_line) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}










