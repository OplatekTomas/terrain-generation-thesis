//
// Created by tomas on 02.10.21.
//

#ifndef VUT_BP_RENDERER_H
#define VUT_BP_RENDERER_H
#include <QOpenGLWidget>
#include <MainWindow.h>
#include <QOpenGLFunctions>


QT_BEGIN_NAMESPACE
namespace Ui { class Renderer; }
QT_END_NAMESPACE


class Renderer : public QOpenGLWidget, protected QOpenGLFunctions  {
Q_OBJECT

public:
    Renderer(QWidget *parent = nullptr);
private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    QOpenGLFunctions* ctx;

    void tempSetup();
};


#endif //VUT_BP_RENDERER_H
