//
// Created by tomas on 04.10.21.
//

#include <renderer/Camera.h>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QTimer>
#include <memory>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <chrono>
#include <QScreen>

using namespace std::chrono;

namespace MapGenerator {
    Camera::Camera() {
        //this->parent = parent;
        w_down = a_down = s_down = d_down = space_down = ctrl_down = false;
        position = glm::vec3(0.5, 0.7, -0.5);
        up = glm::vec3(0, 1, 0);
        worldUp = up;
        front = glm::vec3(0, 0, 1);
        right = glm::vec3(1, 0, 0);
        pitch = 0;
        yaw = -90.0f;
        xMove = yMove = 0;
        //Right now the camera and position updates every 1/60th of a second (60fps)
        timer = std::make_unique<QTimer>(this);
        auto screen = QGuiApplication::primaryScreen();
        refreshRate = 1000 / screen->refreshRate();
        idleTimeoutIterations = 4000 / refreshRate;
        timer->setInterval((int) refreshRate);
        connect(timer.get(), SIGNAL(timeout()), this, SLOT(updateSteps()));
        timer->start();
    }


    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(position, this->position + this->front, this->up);
    }

    glm::vec3 Camera::getPosition() {
        return position;
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
            case Qt::Key::Key_Shift:
                space_down = pressed;
                break;
            case Qt::Key::Key_Control:
                ctrl_down = pressed;
                break;
        }
        event->accept();
    }

    void Camera::updateSteps() {
        static int iterations = 0;
        auto keyboardMoved = updateKeyboardEvents();
        auto mouseMoved = updateMouseEvents();
        if (keyboardMoved || mouseMoved) {
            iterations = 0;
        } else {
            iterations++;
            if (iterations > idleTimeoutIterations) {
                iterations = 0;
            }
        }
    }

    bool Camera::updateMouseEvents() {
        if (xMove == 0 && yMove == 0) {
            return false;
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
        return true;
    }

    bool Camera::updateKeyboardEvents() {
        auto y = position.y;
        if (w_down) {
            position += front * moveSpeed;
        }
        if (s_down) {
            position -= front * moveSpeed;
        }
        if (d_down) {
            position += right * moveSpeed;
        }
        if (a_down) {
            position -= right * moveSpeed;
        }
        position.y = y;

        if (space_down) {
            position += worldUp * moveSpeed;
        }
        if (ctrl_down) {
            position -= worldUp * moveSpeed;
        }
        return w_down || s_down || d_down || a_down || space_down || ctrl_down;
    }

    void Camera::mouseMoved(QMouseEvent *event) {
        static int prevX = 0;
        static int prevY = 0;
        static milliseconds timeSincePrevious;
        if (prevX == 0 && prevY == 0) {
            prevX = event->x();
            prevY = event->y();
            timeSincePrevious = duration_cast<milliseconds>(system_clock::now().time_since_epoch());;
            return;
        }
        auto dx = prevX - event->x();
        auto dy = prevY - event->y();
        auto dt = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - timeSincePrevious;
        if (dt.count() > 100) { //not a great way to do this, but it works for now
            prevX = prevY = 0;
            return;
        }
        xMove += dx;
        yMove += dy;
        prevX = event->x();
        prevY = event->y();
        timeSincePrevious = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }

    void Camera::scrolled(QWheelEvent *event) {
        if (event->delta() > 0) {
            moveSpeed *= 1.1;
        } else {
            moveSpeed *= 0.9;
        }
    }

    void Camera::print() {
        std::cout << "Camera position: " << position.x << " " << position.y << " " << position.z << std::endl;
        std::cout << "Camera front: " << front.x << " " << front.y << " " << front.z << std::endl;
        std::cout << "Camera up: " << up.x << " " << up.y << " " << up.z << std::endl;
    }

    glm::vec2 Camera::getPositionOnScreen(glm::vec2 pos) {
        return pos;

    }


}

