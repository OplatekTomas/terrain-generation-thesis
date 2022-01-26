//
// Created by tomas on 04.10.21.
//

#include <renderer/Camera.h>
#include <QKeyEvent>
#include <QTimer>
#include <memory>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

namespace MapGenerator {
    Camera::Camera(Renderer *parent) {
        this->parent = parent;
        w_down = a_down = s_down = d_down = space_down = ctrl_down =  false;
        position = glm::vec3(0.5,0.7,-0.5);
        up = glm::vec3(0, 1, 0);
        worldUp = up;
        front = glm::vec3(0, 0, 1);
        right = glm::vec3(1, 0, 0);
        pitch = 0;
        yaw = -90.0f;
        xMove = yMove = 0;
        //Right now the camera and position updates every 1/60th of a second (60fps)
        timer = std::make_unique<QTimer>(this);
        timer->setInterval(16);
        connect(timer.get(), SIGNAL(timeout()), this, SLOT(updateSteps()));
        timer->start();
    }


    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }

    void Camera::keyEvent(QKeyEvent *event) {
        auto pressed = event->type() == QEvent::KeyPress;
        switch (event->key()) {
            case Qt::Key::Key_W:
                w_down = pressed;
                break;
            case Qt::Key::Key_A:
                a_down = pressed;
                break;
            case Qt::Key::Key_S:
                s_down = pressed;
                break;
            case Qt::Key::Key_D:
                d_down = pressed;
                break;
            case Qt::Key::Key_Space:
                space_down = pressed;
                break;
            case Qt::Key::Key_Control:
                ctrl_down = pressed;
                break;
        }
        event->accept();
    }

    void Camera::updateSteps() {
        updateKeyboardEvents();
        updateMouseEvents();
    }

    void Camera::updateMouseEvents() {
        if(xMove == 0 && yMove == 0){
            return;
        }
        const auto sensitivity = 0.2f;

        yaw += xMove * sensitivity;
        pitch += yMove * sensitivity;
        pitch = pitch > 89.0f ? 89.0f : pitch < -89.0f ? -89.0f : pitch;
        xMove = 0;
        yMove = 0;
        glm::vec3 frontTmp;
        frontTmp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frontTmp.y = sin(glm::radians(pitch));
        frontTmp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = -glm::normalize(frontTmp);
        // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    void Camera::updateKeyboardEvents() {
        auto y = position.y;
        const float stepSize = 0.03f;
        if (w_down) {
            position += front * stepSize;
        }
        if (s_down) {
            position -= front * stepSize;
        }
        if (d_down) {
            position += right * stepSize;
        }
        if (a_down) {
            position -= right * stepSize;
        }
        position.y = y;

        if (space_down) {
            position += worldUp * stepSize;
        } if (ctrl_down) {
            position -= worldUp * stepSize;
        }

    }

    void Camera::mouseMoved(QMouseEvent *event) {
        static int prevX = 0;
        static int prevY = 0;
        if (event->buttons() != Qt::LeftButton) {
            prevX = prevY = 0;
            return;
        }

        if (prevX == 0 && prevY == 0) {
            prevX = event->x();
            prevY = event->y();
            return;
        }
        xMove += prevX - event->x();
        yMove += prevY - event->y();
        prevX = event->x();
        prevY = event->y();
    }


}

