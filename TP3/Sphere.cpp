#include "Sphere.h"

Sphere::Sphere()
    :indexBuf(QOpenGLBuffer::IndexBuffer){
    arrayBuf.create();
    indexBuf.create();
}

void Sphere::init(){

    initializeOpenGLFunctions();

    GameObject::init();

    std::vector<QVector3D> vertices;
    std::vector<std::vector<GLushort>> indices;
    OBJIO::open("sphere.obj",vertices,indices);

    size_vertices = vertices.size();
    size_indices = indices.size() * 3;
    this->indexSize += size_indices;

    t_vertices = new QVector3D[vertices.size()];
    t_indices = new GLushort[indices.size() * 3];

    for(size_t i = 0; i < vertices.size(); i++){
        t_vertices[i] = vertices[i];
    }
    for(size_t i = 0; i < indices.size(); i++){
        for(size_t j = 0; j < indices[i].size(); j++){
            t_indices[i * 3 + j] = indices[i][j];
        }
    }


}

void Sphere::update(){

    GameObject::update();
    for(int i = 0; i < size_vertices; i++){
        QVector4D transformedPoint;
        //this->getTransform().apply();
        transformedPoint = this->getTransform().getAppliedMatrix() * (QVector4D(t_vertices[i], 1.0)) ;
        t_vertices[i] = QVector3D(transformedPoint);
    }

    arrayBuf.bind();
    arrayBuf.allocate(t_vertices, size_vertices * sizeof(QVector3D));

    indexBuf.bind();
    indexBuf.allocate(t_indices, size_indices * sizeof(GLushort));


}


void Sphere::render(QOpenGLShaderProgram *program){

    GameObject::render(program);

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
    glDrawElements(GL_TRIANGLES, size_indices , GL_UNSIGNED_SHORT, 0);



}
