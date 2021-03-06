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
 /*
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]
*/
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

    // Lighting
    /*
    glEnable(GL_LIGHT1);
    GLfloat lightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat lightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[]= { 3.0f, 3.0f, -5.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
*/

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

   if(this->mode_libre){
       zNear = 1.0; zFar = 7.0; fov = 0.0;
   }
   else{
       zNear = 1.0; zFar = 7.0* this->geometries->ratio; fov = 45.0;
   }
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

/*
    qglColor(Qt::white);
    renderText(20, 20, QString("FPS:%1").arg(last_count));
*/


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
    if(this->mode_libre){
        model.translate(mouvement_x,mouvement_y,mouvement_z);   //movement de la caméra (mode libre)
    }

    if(!this->mode_libre){
        model.rotate(timer_rotation,QVector3D(0.0f,1.0f,0.0f)); //rotation automatique dans le mode orbital
    }

    model.rotate(-90,QVector3D(1.0,0.0,0.0));   //pour dresser le plan
    model.translate(-0.5 * this->geometries->ratio,-0.5* this->geometries->ratio,0.0);             //mettre le plan au milieu de la scène

    if(!this->mode_libre){
        view.lookAt(QVector3D(0,1.0 * this->geometries->ratio,1.5* this->geometries->ratio), QVector3D(0,0,0.0), QVector3D(0.0,1.0,0.0));
    }
    else{
        view.lookAt(QVector3D(0,1.0 * this->geometries->ratio/5 ,1.5 * this->geometries->ratio/5), QVector3D(0,0,0.0), QVector3D(0.0,1.0,0.0));
    }

    matrixMVP = this->projection * view * model;

    program.setUniformValue("mvp_matrix", matrixMVP);
//! [6]

    program.setUniformValue("texture_grass", 1);
    program.setUniformValue("texture_rock", 2);
    program.setUniformValue("texture_snow", 3);


    glEnable(GL_LIGHTING);
    geometries->drawPlane(&program);
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
        vitesse_rotation+=0.5f;
        break;
    case Qt::Key::Key_Down:
        vitesse_rotation-=0.5f;
        break;
    case Qt::Key::Key_C :
        if(mode_libre) mode_libre = false;
        else mode_libre = true;
        break;
    case Qt::Key::Key_P :
        if(this->geometries->polygone_line == true){
            this->geometries->polygone_line = false;
        }
        else{
            this->geometries->polygone_line = true;
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
