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
        timer = std::unique_ptr<QTimer>(new QTimer(this));
        timer->setInterval(16);
        connect(timer.get(), SIGNAL(timeout()), this, SLOT(updateSteps()));
        timer->start();
        w_down = a_down = s_down = d_down = false;
        position = glm::vec3(0.5,0.5,2);
        up = glm::vec3(0, 1, 0);
        worldUp = up;
        front = glm::vec3(0, 0, -1);
        right = glm::vec3(1, 0, 0);
        pitch = 0;
        yaw = -90.0f;
        xMove = yMove = 0;
        moved = false;
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

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(this->position, this->position + this->front, this->up);
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
        updateKeyboardEvents();
        updateMouseEvents();
        if (moved) {
            parent->renderNow();
        }
        moved = false;
    }

    void Camera::updateMouseEvents() {
        if(xMove == 0 && yMove == 0){
            return;
        }
        moved = true;
        const auto sensitivity = 0.2f;

        yaw += xMove * sensitivity;
        pitch -= yMove * sensitivity;
        pitch = pitch > 89.0f ? 89.0f : pitch < -89.0f ? -89.0f : pitch;
        xMove = 0;
        yMove = 0;
        glm::vec3 frontTmp;
        frontTmp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frontTmp.y = sin(glm::radians(pitch));
        frontTmp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(frontTmp);
        // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    void Camera::updateKeyboardEvents() {
        const float stepSize = 0.05;
        if (w_down) {
            position += front * stepSize;
            moved = true;
        }
        if (s_down) {
            position -= front * stepSize;
            moved = true;
        }
        if (a_down) {
            position -= right * stepSize;
            moved = true;
        }
        if (d_down) {
            position += right * stepSize;
            moved = true;
        }
        position.y = 0.5f;

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

