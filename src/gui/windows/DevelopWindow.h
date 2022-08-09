//
// Created by tomas on 2.8.22.
//

#ifndef VUT_BP_DEVELOPWINDOW_H
#define VUT_BP_DEVELOPWINDOW_H

#include "renderer/Scene.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DevelopWindow;
}
QT_END_NAMESPACE

class DevelopWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DevelopWindow(QWidget* parent = nullptr);

    ~DevelopWindow() override;

private:
    std::shared_ptr<MapGenerator::Renderer::Scene> createScene();
    Ui::DevelopWindow* ui;
};

#endif // VUT_BP_DEVELOPWINDOW_H
