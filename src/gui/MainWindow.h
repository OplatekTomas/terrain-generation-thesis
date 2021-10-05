//
// Created by tomas on 02.10.21.
//

#pragma once

#include <QMainWindow>
#include <opengl/Renderer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Renderer;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    Renderer* renderer;
    QLayout* baseGridLayout;

};


