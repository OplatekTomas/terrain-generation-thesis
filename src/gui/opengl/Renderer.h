//
// Created by tomas on 02.10.21.
//

#pragma once


#include <QOpenGLWidget>
#include <MainWindow.h>
#include <QOpenGLFunctions>
#include <opengl/Camera.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Renderer; }
QT_END_NAMESPACE

class Camera;

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions  {
Q_OBJECT

public:
    Renderer(Camera* camera,QWidget *parent = nullptr);
private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void tempSetup();
};


