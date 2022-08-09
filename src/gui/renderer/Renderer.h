//
// Created by tomas on 2.8.22.
//

#pragma once

#include <renderer/Scene.h>
#include "renderer/SceneObject.h"
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <geGL/OpenGLContext.h>
#include <geGL/geGL.h>
#include <memory>
#include <renderer/GBuffer.h>

namespace MapGenerator::Renderer {

    class Renderer : public QOpenGLWidget {
        Q_OBJECT
    public:
        explicit Renderer(QWidget* parent) : QOpenGLWidget(parent) {
            scene = std::make_shared<Scene>();
        }

        void setScene(std::shared_ptr<Scene> scene);

    protected:
        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w, int h) override;

    private:
        void geometryPass();

        QOpenGLContext* context;
        GBuffer gBuffer;

        std::shared_ptr<Scene> scene;

        std::shared_ptr<ge::gl::Context> gl;
    };

} // namespace MapGenerator::Renderer
