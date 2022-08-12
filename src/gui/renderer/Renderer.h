//
// Created by tomas on 2.8.22.
//

#pragma once

#include "windows/RenderOptions.h"
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <chrono>
#include <geGL/OpenGLContext.h>
#include <geGL/geGL.h>
#include <iostream>
#include <memory>
#include <qobjectdefs.h>
#include <renderer/GBuffer.h>
#include <renderer/Scene.h>
#include <renderer/SceneObject.h>
#include <renderer/input/InputHandler.h>

namespace MapGenerator::Renderer {

    class Renderer : public QOpenGLWidget {
        Q_OBJECT
    public:
        explicit Renderer(QWidget* parent);
        ~Renderer();
        void setScene(std::shared_ptr<Scene> scene);

    protected:
        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w, int h) override;

        signals:

        void wheelEvent(QWheelEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;


    private:
        void geometryPass();

        void setUpInputHandler();

        QOpenGLContext* context;
        GBuffer gBuffer;
        RenderOptions* options;
        std::shared_ptr<Scene> scene;

        std::shared_ptr<InputHandler> inputHandler;

        std::chrono::time_point<std::chrono::high_resolution_clock> frameStartTime;
        float lastFrameTime;

        std::shared_ptr<ge::gl::Context> gl;
    };

} // namespace MapGenerator::Renderer
