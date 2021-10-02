//
// Created by tomas on 02.10.21.
//

#ifndef VUT_BP_MAINWINDOW_H
#define VUT_BP_MAINWINDOW_H

#include <QMainWindow>
#include "opengl/Renderer.h"

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


#endif //VUT_BP_MAINWINDOW_H
