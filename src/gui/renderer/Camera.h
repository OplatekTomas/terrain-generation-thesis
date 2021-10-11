//
// Created by tomas on 04.10.21.
//

#pragma once

#include <glm/vec3.hpp>
#include <QObject>
#include <QKeyEvent>
#include <memory>
#include <QtCore>

namespace MapGenerator {
    class Camera : public QObject {
    Q_OBJECT
    public:
        explicit Camera();

    public slots:
        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);

        void updateValues();

    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        float yaw;
        float pitch;
        std::unique_ptr<QTimer> timer;
        bool w_down;
        bool a_down;
        bool s_down;
        bool d_down;

        void updateSteps();
    };

}
