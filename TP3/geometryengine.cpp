
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


    OBJIO::open("cube.obj", vertices, indices);

    std::cout<<vertices.size()<<"vertices"<<std::endl;
    std::cout<<indices.size()<<"faces"<<std::endl;
/*
    for(size_t i = 0; i < vertices.size(); i++){
        std::cout<<i<<":"<<vertices[i].x()<<","<<vertices[i].y()<<","<<vertices[i].z()<<std::endl;
    }
*/
    /*
        for(size_t i = 0; i < indices.size(); i++){
            for(size_t j = 0; j < indices[i].size(); j++){
                std::cout<<indices[i][j]<<"-";
            }
            std::cout<<std::endl;
        }
    */

    const int size_vertices = vertices.size();
    const int size_indices = indices.size() * 3;
    QVector3D* t_vertices = new QVector3D[size_vertices];
    GLushort* t_indices = new GLushort[size_indices];

    for(size_t i = 0; i < vertices.size(); i++){
        t_vertices[i] = vertices[i];
    }
    for(size_t i = 0; i < indices.size(); i++){
        for(size_t j = 0; j < indices[i].size(); j++){
            t_indices[i * 3 + j] = indices[i][j];
        }
    }


    QVector3D test_vertices[] = {
        // Vertex data for face 0
        QVector3D(-1.0f, -1.0f,  1.0f),
        QVector3D( 1.0f, -1.0f,  1.0f),
        QVector3D(-1.0f,  1.0f,  1.0f),
        QVector3D( 1.0f,  1.0f,  1.0f),
        // Vertex data for face 1
        QVector3D( 1.0f, -1.0f,  1.0f),
        QVector3D( 1.0f, -1.0f, -1.0f),
        QVector3D( 1.0f,  1.0f,  1.0f),
        QVector3D( 1.0f,  1.0f, -1.0f),

        // Vertex data for face 2
        QVector3D( 1.0f, -1.0f, -1.0f),
        QVector3D(-1.0f, -1.0f, -1.0f),
        QVector3D( 1.0f,  1.0f, -1.0f),
        QVector3D(-1.0f,  1.0f, -1.0f),

        // Vertex data for face 3
        QVector3D(-1.0f, -1.0f, -1.0f),
        QVector3D(-1.0f, -1.0f,  1.0f),
        QVector3D(-1.0f,  1.0f, -1.0f),
        QVector3D(-1.0f,  1.0f,  1.0f),

        // Vertex data for face 4
        QVector3D(-1.0f, -1.0f, -1.0f),
        QVector3D( 1.0f, -1.0f, -1.0f),
        QVector3D(-1.0f, -1.0f,  1.0f),
        QVector3D( 1.0f, -1.0f,  1.0f),

        // Vertex data for face 5
        QVector3D(-1.0f,  1.0f,  1.0f),
        QVector3D( 1.0f,  1.0f,  1.0f),
        QVector3D(-1.0f,  1.0f, -1.0f),
        QVector3D( 1.0f,  1.0f, -1.0f),
    };

         GLushort test_indices[] = {
                  0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
                  4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
                  8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
                 12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
                 16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
                 20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
             };

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(&test_vertices, 24 * sizeof(QVector3D));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(&test_indices, 34 * sizeof(GLushort));
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
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
/*
    // Offset for texture coordinate
    offset += sizeof(QVector3D);


    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
*/
    // Draw cube geometry using indices from VBO 1


    glDrawElements(GL_TRIANGLES, 34, GL_UNSIGNED_SHORT, 0);


    if(this->polygone_line) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}










