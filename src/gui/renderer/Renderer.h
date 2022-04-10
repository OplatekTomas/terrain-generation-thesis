//
// Created by tomas on 06.10.21.
//

#pragma once


#include <QExposeEvent>
#include <geGL/OpenGLContext.h>
#include <QWidget>
#include <QSurfaceFormat>
#include <QScreen>
#include <renderer/SSAO.h>
#include <QWindow>
#include <renderer/Skybox.h>
#include <ui/BaseWidget.h>
#include <QOpenGLContext>
#include <renderer/Camera.h>
#include <MapGenerator.h>
#include <QThread>
#include <QResizeEvent>
#include <renderer/Scene3D.h>
#include <QBackingStore>
#include <QPushButton>

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

        void wheelEvent(QWheelEvent *) override;


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

        void initializeGBuffer();

        void initializeLightning();

        void drawQuad();

        void resizeRender();

        void swapUi();

        string readShader(string path);

        void render(QPainter *painter);

        QBackingStore *m_backingStore;


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

        std::shared_ptr<SSAO> ssao;
        std::shared_ptr<class Skybox> skybox;


        bool drawGui = true;


        BaseWidget *background;

    };

}



