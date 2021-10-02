//
// Created by tomas on 02.10.21.
//

#ifndef VUT_BP_RENDERER_H
#define VUT_BP_RENDERER_H
#include <QOpenGLWidget>
#include <MainWindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Renderer; }
QT_END_NAMESPACE


class Renderer : public QOpenGLWidget  {
Q_OBJECT

public:
    Renderer(QWidget *parent = nullptr);
};


#endif //VUT_BP_RENDERER_H
