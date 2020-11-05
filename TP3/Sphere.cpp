#include "Sphere.h"

Sphere::Sphere(){

}

void Sphere::init(QOpenGLBuffer& arrayBuf, QOpenGLBuffer& indexBuf){

    std::vector<QVector3D> vertices;
    std::vector<std::vector<GLushort>> indices;
    OBJIO::open("sphere.obj",vertices,indices);

    size_vertices = vertices.size();
    size_indices = indices.size() * 3;

    t_vertices = new QVector3D[size_vertices];
    t_indices = new GLushort[size_indices];

    for(size_t i = 0; i < vertices.size(); i++){
        t_vertices[i] = vertices[i];
    }
    for(size_t i = 0; i < indices.size(); i++){
        for(size_t j = 0; j < indices[i].size(); j++){
            t_indices[i * 3 + j] = indices[i][j];
        }
    }

    arrayBuf.bind();
    arrayBuf.allocate(t_vertices, size_vertices * sizeof(QVector3D));

    indexBuf.bind();
    indexBuf.allocate(t_indices, size_indices * sizeof(GLushort));
}

void Sphere::update(){

}


void Sphere::render(QOpenGLShaderProgram *program, QOpenGLBuffer& arrayBuf, QOpenGLBuffer& indexBuf){

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));
/*
    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
*/
    // Draw cube geometry using indices from VBO 1


}
