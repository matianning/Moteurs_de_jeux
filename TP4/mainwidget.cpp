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

#include "mainwidget.h"

#include <QMouseEvent>
#include <math.h>
#include <QPainter>


MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0),
    frameCount(0)
{
       last_count = 0;
       last_time = QTime::currentTime();
       QTimer *timer = new QTimer(this);
       connect(timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
       timer->start(100);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        mouvement_rotation += 10;
        // Request an update
        update();
    }
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    geometries = new GeometryEngine;

    // Use QBasicTimer because its faster than QTimer
    //timer.start(12, this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    timer->start(100);


}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    texture_grass = new QOpenGLTexture(QImage(":/grass.png").mirrored());
    texture_rock = new QOpenGLTexture(QImage(":/rock.png").mirrored());
    texture_snow = new QOpenGLTexture(QImage(":/snowrocks.png").mirrored());

    texture_grass->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_grass->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_grass->setWrapMode(QOpenGLTexture::Repeat);

    texture_rock->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_rock->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_rock->setWrapMode(QOpenGLTexture::Repeat);

    texture_snow->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_snow->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_snow->setWrapMode(QOpenGLTexture::Repeat);
}


void MainWidget::resizeGL(int w, int h)
{
   qreal aspect = qreal(w) / qreal(h ? h : 1);
   qreal zNear = 1.0, zFar = 7.0, fov = 45.0;


   zNear = 0.1; zFar = 7.0* this->geometries->ratio; fov = 45.0;

    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}


void MainWidget::paintGL()
{
    frameCount++;

    QTime new_time = QTime::currentTime();
    if (last_time.msecsTo(new_time) >= 1000)
    {
    // sauvegarder le FPS dans last_count et on réinitialise
        last_count = frameCount;
        frameCount = 0;
        last_time = QTime::currentTime();
    }

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture_grass->bind(1);
    texture_rock->bind(2);
    texture_snow->bind(3);


//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrixMVP;


    QMatrix4x4 view, model;

    model.setToIdentity();
    view.setToIdentity();

    //==============Transformation du monde===============

    model.translate(mouvement_x,mouvement_y,mouvement_z);   //movement de la caméra (mode libre)


    model.rotate(-90,QVector3D(1.0,0.0,0.0));   //pour dresser le plan
    model.translate(-0.5 * this->geometries->ratio,-1.0* this->geometries->ratio,0.0);             //mettre le plan au milieu de la scène

    view.lookAt(QVector3D(0,1.0 * this->geometries->ratio,1.5* this->geometries->ratio), QVector3D(0,0,0.0), QVector3D(0.0,1.0,0.0));

    matrixMVP = this->projection * view * model;

    program.setUniformValue("mvp_matrix", matrixMVP);

//! [6]

    program.setUniformValue("texture_grass", 1);
    program.setUniformValue("texture_rock", 2);
    program.setUniformValue("texture_snow", 3);








    //std::cout<<geometries->sizeObject()<<std::endl;

    float isPlane = 1.0f;

    glEnable(GL_LIGHTING);
    program.setUniformValue("isPlane",isPlane);
    geometries->getGameObjects().at(0).render(&program, projection);






    //============Transformation de sphere================

    matrixMVP = this->projection * view * model;
    //program.setUniformValue("mvp_matrix", matrixMVP);
    isPlane = 0.0f;
    program.setUniformValue("isPlane",isPlane);

    //**************Calculer le centre de la sphère*******************
    geometries->getGameObjects().at(1).calculateCenter(projection);
    QVector3D sphereCenter = geometries->getGameObjects().at(1).getCenter();

    //**************Adjuster l'hauteur avec celle de plane************
    ball_y = geometries->getGameObjects().at(0).getHauteur(sphereCenter);
    geometries->getGameObjects().at(1).move(ball_x,ball_y, ball_z);
    geometries->getGameObjects().at(1).render(&program, matrixMVP);

}

void MainWidget::keyPressEvent(QKeyEvent* e){
    switch (e->key()) {
    case Qt::Key::Key_Z :
        mouvement_z+=0.05f;
        break;
    case Qt::Key::Key_S :
        mouvement_z-=0.05f;
        break;
    case Qt::Key::Key_Q :
        mouvement_x+=0.05f;
        break;
    case Qt::Key::Key_D :
        mouvement_x-=0.05f;
        break;
    case Qt::Key::Key_U :
        mouvement_y+=0.1f;
        break;
    case Qt::Key::Key_I :
        mouvement_y-=0.1f;
        break;
    case Qt::Key::Key_Up:
        ball_z-=0.1f;
        break;
    case Qt::Key::Key_Down:
        ball_z+=0.1f;
        break;
    case Qt::Key::Key_Left:
        ball_x-=0.1f;
        break;
    case Qt::Key::Key_Right:
        ball_x+=0.1f;
        break;
    case Qt::Key::Key_C :
        if(mode_libre) mode_libre = false;
        else mode_libre = true;
        break;
    case Qt::Key::Key_P :

        if(this->geometries->getGameObjects().at(0).polygone_line == true){
            for(size_t i = 0; i < this->geometries->getGameObjects().size(); i++){
                this->geometries->getGameObjects().at(i).polygone_line = false;
            }

        }
        else{
            for(size_t i = 0; i < this->geometries->getGameObjects().size(); i++){
                this->geometries->getGameObjects().at(i).polygone_line = true;
            }

        }

        break;
    }
    repaint();
}

void MainWidget::updateAnimation()
{
    timer_rotation+=vitesse_rotation;
    this->update();
}
