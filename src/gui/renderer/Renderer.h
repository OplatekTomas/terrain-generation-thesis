//
// Created by tomas on 06.10.21.
//

#pragma once


#include <QExposeEvent>
#include <geGL/OpenGLContext.h>
#include <QWidget>
#include <QSurfaceFormat>
#include <QScreen>

#include <QWindow>
#include <QOpenGLContext>
#include <renderer/Camera.h>
#include <MapGenerator.h>
#include <QThread>
#include <QResizeEvent>
#include <renderer/Scene3D.h>

namespace MapGenerator {
    class Camera;

    class Scene3D;

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

        bool setupLib();

        void clearView();

        void initializeGBufferTextures();

        void lightningPass();

        void checkForErrors();

        void geometryPass();

        void ssaoPass();

        void initializeGBuffer();

        void initializeLightning();

        void initializeSsao();

        void drawQuad();


        std::shared_ptr<MapGenerator> mapGenerator;
        std::shared_ptr<Scene3D> scene;
        bool renderLoopRunning = false;
        double refreshRate;
        bool initialized;
        QOpenGLContext *context;
        std::shared_ptr<ge::gl::Context> gl;
        std::shared_ptr<ge::gl::Framebuffer> gBuffer;
        std::shared_ptr<ge::gl::Texture> gPosition;
        std::shared_ptr<ge::gl::Texture> gNormal;
        std::shared_ptr<ge::gl::Texture> gAlbedo;
        shared_ptr<ge::gl::VertexArray> quadVAO;
        shared_ptr<ge::gl::Buffer> quadBuffer;
        shared_ptr<ge::gl::Renderbuffer> rboDepth;
        shared_ptr<ge::gl::Shader> lightningVS;
        shared_ptr<ge::gl::Shader> lightningFS;
        shared_ptr<ge::gl::Program> lightningProgram;
        std::shared_ptr<Camera> camera;
        QSurfaceFormat surfaceFormat;
        std::unique_ptr<QTimer> renderTimer;
        shared_ptr<ge::gl::Texture> noiseTexture;
        shared_ptr<ge::gl::Framebuffer> ssaoFBO;
        shared_ptr<ge::gl::Texture> ssaoColorBuffer;
        std::vector<glm::vec3> ssaoKernel;
        shared_ptr<ge::gl::Shader> ssaoFS;
        shared_ptr<ge::gl::Shader> ssaoVS;
        shared_ptr<ge::gl::Program> ssaoProgram;
    };

}



