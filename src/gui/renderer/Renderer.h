//
// Created by tomas on 10.04.22.
//


#pragma once


#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <memory>
#include <QExposeEvent>
#include "geGL/OpenGLContext.h"
#include <QWidget>
#include <QSurfaceFormat>
#include <renderer/SSAO.h>
#include <renderer/Skybox.h>
#include <renderer/Camera.h>
#include <MapGenerator.h>
#include <renderer/Scene3D.h>

using namespace MapGenerator;

    class Renderer : public QOpenGLWidget {
        Q_OBJECT
    public:
        Renderer(QWidget *parent) : QOpenGLWidget(parent) {}

        bool isInitialized() { return initialized; }

        void startGeneration(GeneratorOptions options, LibConfig config);


    protected:
        void initializeGL() override;

        void paintGL() override;

        void resizeGL(int w, int h) override;

        bool event(QEvent *event) override;

    public slots:

        void keyPressEvent(QKeyEvent *event) override;

        void keyReleaseEvent(QKeyEvent *event) override;


    private:
        void initializeLightning();

        void initializeGBufferTextures();

        void initializeEffects();

        void initializeCamera();

        void initializeGBuffer();

        void lightningPass();

        void drawQuad();

        void geometryPass();


        bool initialized;
        std::shared_ptr<class MapGenerator> mapGenerator;
        std::shared_ptr<Scene3D> scene;
        std::shared_ptr<Camera> camera;

        QOpenGLContext *context;
        QSurfaceFormat surfaceFormat;
        std::shared_ptr<ge::gl::Context> gl;
        std::shared_ptr<ge::gl::Framebuffer> gBuffer;
        std::shared_ptr<ge::gl::Texture> gPosition;
        std::shared_ptr<ge::gl::Texture> gNormal;
        std::shared_ptr<ge::gl::Texture> gAlbedo;
        std::shared_ptr<ge::gl::VertexArray> quadVAO;
        std::shared_ptr<ge::gl::Buffer> quadBuffer;
        std::shared_ptr<ge::gl::Renderbuffer> rboDepth;
        std::shared_ptr<ge::gl::Shader> lightningVS;
        std::shared_ptr<ge::gl::Shader> lightningFS;
        std::shared_ptr<ge::gl::Program> lightningProgram;
        std::shared_ptr<SSAO> ssao;
        std::shared_ptr<class Skybox> skybox;


        void checkGLError();
    };





