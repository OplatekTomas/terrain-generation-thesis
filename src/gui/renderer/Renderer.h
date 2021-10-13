//
// Created by tomas on 06.10.21.
//

#pragma once


#include <QExposeEvent>
#include <geGL/OpenGLContext.h>
#include <QWidget>
#include <QSurfaceFormat>
#include <QWindow>
#include <QOpenGLContext>
#include <renderer/Camera.h>
#include <MapGenerator.h>
#include <QThread>
#include <QResizeEvent>

namespace MapGenerator {
    class Camera;
    class Renderer : public QWindow {
    Q_OBJECT
    public:
        explicit Renderer(QWindow *parent = nullptr);

        virtual void render();

        virtual void initialize();

    public slots:

        void renderNow();

    signals:

        void keyPressEvent(QKeyEvent *event) override;

        void keyReleaseEvent(QKeyEvent *event) override;
        void mouseMoveEvent(QMouseEvent *) override;
        void resizeEvent(QResizeEvent *) override;

    protected:
        bool event(QEvent *event) override;

        void exposeEvent(QExposeEvent *event) override;

        std::shared_ptr<MapGenerator> mapGenerator;

        bool initialized;
        int drawCount;
        QOpenGLContext *context;
        std::shared_ptr<ge::gl::Context> gl;
        std::shared_ptr<ge::gl::Buffer> indices;
        std::shared_ptr<ge::gl::Buffer> vertices;
        std::shared_ptr<ge::gl::Program> shaderProgram;
        std::shared_ptr<ge::gl::VertexArray> vao;
        std::shared_ptr<Camera> camera;

        QSurfaceFormat surfaceFormat;

        QThread *renderThread;

        bool setupLib();

    };

}



