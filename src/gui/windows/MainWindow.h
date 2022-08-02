//
// Created by tomas on 02.10.21.
//

#pragma once

#include <QMainWindow>
#include <QOpenGLContext>
#include <QOpenGLWidget>
//#include "renderer/Renderer.h"
#include "views/rendererlayout.h"
#include "views/mapview.h"
#include "views/rendererlayout.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    bool eventFilter(QObject *obj, QEvent *event) override;

    void drawRenderer(glm::vec4 box);

    void drawMap();

private:
    Ui::MainWindow *ui;
    //Renderer* renderer;
    QLayout *baseGridLayout;


    void onMapLoaded();

    bool isRendering;
    RendererLayout *rendererLayout;
    MapView *mapView;

};


