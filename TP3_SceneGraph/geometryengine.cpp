/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()    //RENDER ENGINE
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{

    initializeOpenGLFunctions();


       // arrayBuf.create();
       // indexBuf.create();




    // Initializes cube geometry and transfers it to VBOs
    initGeometry();
}

GeometryEngine::~GeometryEngine()
{

       // arrayBuf.destroy();
       // indexBuf.destroy();


}
//! [0]

void GeometryEngine::initGeometry()
{

    GameObject * soleil = new Sphere();
    GameObject * terre = new Sphere();
    GameObject * lune = new Sphere();

    lune->init();

    lune->g_translate(QVector3D(1.5,0.0,0.0));




    terre->init();
    terre->addChild(lune);
    terre->g_translate(QVector3D(3.0,0.0,0.0));
    terre->g_scale(QVector3D(0.5,0.5,0.5));
    lune->g_rotate(QVector3D(0.0,0.0,60.0));
    lune->g_scale(QVector3D(0.2,0.2,0.2));



    soleil->init();
    soleil->addChild(terre);
    soleil->g_scale(QVector3D(2.0f,2.0f,2.0f));
    //soleil->g_rotate(QVector3D(0.0,0.0,0.0));


    soleil->update();


    gameobjects.push_back(soleil);










}

//! [2]
void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program)
{
    size = 0;    //reInitialiser le size

    for(size_t i = 0; i < gameobjects.size(); i++){
       gameobjects[i]->render(program);

        size += gameobjects[i]->indexSize;
        //std::cout<<"Size : "<<size<<std::endl;
    }

   // std::cout<<"Size GameObjects : "<<gameobjects.size()<<std::endl;
    //std::cout<<"Size total Index : "<<this->size<<std::endl;



    //glDrawElements(GL_TRIANGLES, this->size , GL_UNSIGNED_SHORT, 0);
    if(this->polygone_line) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
//! [2]
