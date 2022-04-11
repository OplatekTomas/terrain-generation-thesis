//
// Created by tomas on 04.10.21.
//

#pragma once

#include <glm/vec3.hpp>
#include <QObject>
#include <QKeyEvent>
#include <memory>
#include <QtCore>
#include <QMouseEvent>
#include <glm/fwd.hpp>


namespace MapGenerator {
    class Camera : public QObject {
    Q_OBJECT


    public:
        Camera();
        glm::vec3 getPosition();

        glm::mat4 getViewMatrix();
        void print();

    public slots:

        void keyEvent(QKeyEvent *event);
        void mouseMoved(QMouseEvent *event);
        void scrolled(QWheelEvent *event);

        void updateSteps();

    private:
        bool updateKeyboardEvents();

        bool updateMouseEvents();
        bool rotate = false;
        double refreshRate;
        int idleTimeoutIterations;
        //Renderer *parent;
        std::unique_ptr<QTimer> timer;
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        float yaw;
        float pitch;
        bool w_down;
        bool a_down;
        bool s_down;
        bool d_down;
        bool ctrl_down;
        bool space_down;
        int xMove;
        int yMove;

        float moveSpeed = 0.03;

    };

}
