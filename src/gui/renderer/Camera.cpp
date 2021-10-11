//
// Created by tomas on 04.10.21.
//

#include <renderer/Camera.h>
#include <QKeyEvent>
#include <QTimer>
#include <memory>

namespace MapGenerator{
    Camera::Camera() {
        timer = std::unique_ptr<QTimer>(new QTimer(this));
        timer->setInterval(16);
        connect(timer.get(), SIGNAL(timeout()), this, SLOT(updateValues()));
        w_down = a_down = s_down = d_down = false;
        position = glm::vec3(0);
        up = glm::vec3(0,1,0);
        front = glm::vec3(0,0,-1);

    }

    void Camera::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
            case Qt::Key::Key_W:
                w_down = true;
                break;
            case Qt::Key::Key_A:
                a_down = true;
                break;
            case Qt::Key::Key_S:
                s_down = true;
                break;
            case Qt::Key::Key_D:
                d_down = true;
                break;
        }
        event->accept();
    }

    void Camera::keyReleaseEvent(QKeyEvent *event) {
        switch (event->key()) {
            case Qt::Key::Key_W:
                w_down = false;
                break;
            case Qt::Key::Key_A:
                a_down = false;
                break;
            case Qt::Key::Key_S:
                s_down = false;
                break;
            case Qt::Key::Key_D:
                d_down = false;
                break;
        }
        event->accept();
    }

    void Camera::updateSteps() {
        updateValues();
    }

    void Camera::updateValues(){
        const float stepSize = 0.05;
        if(w_down){

        }
    }

}

