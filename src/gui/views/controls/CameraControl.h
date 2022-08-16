
#pragma once

#include <QTimer>
#include <QWidget>
#include <memory>
#include <qwidget.h>
#include <renderer/scene_objects/Camera.h>

using namespace MapGenerator::Renderer::SceneObjects;

QT_BEGIN_NAMESPACE
namespace Ui {
    class CameraControl;
}
QT_END_NAMESPACE


class CameraControl : public QWidget {
    Q_OBJECT
public:
    CameraControl();
    CameraControl(std::shared_ptr<Camera> camera, QWidget* parent);
    ~CameraControl();

protected slots:
    void update();

private:
    Ui::CameraControl* ui;

    std::shared_ptr<Camera> camera;
    QTimer* timer;
};
