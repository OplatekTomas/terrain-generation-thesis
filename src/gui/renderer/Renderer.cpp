//
// Created by tomas on 2.8.22.
//
#include "renderer/input/InputHandler.h"
#include "windows/RenderOptions.h"
#include <QMouseEvent>
#include <StaticFunctions.h>
#include <chrono>
#include <geGL/geGL.h>
#include <qapplication.h>
#include <qevent.h>
#include <qobject.h>
#include <qopenglwidget.h>
#include <renderer/Renderer.h>

namespace MapGenerator::Renderer {

    Renderer::Renderer(QWidget* parent) : QOpenGLWidget(parent) {
        this->scene = std::make_shared<Scene>();

        setUpInputHandler();
        this->setFocusPolicy(Qt::StrongFocus);

        options = new RenderOptions(this);
        options->show();
    }

    void Renderer::setUpInputHandler() {
        this->inputHandler = std::make_shared<InputHandler>();
        connect(this, &Renderer::mouseMoveEvent, [this](QMouseEvent* event) {
            this->inputHandler->mouseMove(event->x(), event->y());
        });

        connect(this, &Renderer::mousePressEvent, [this](QMouseEvent* event) {
            this->inputHandler->mousePress((InputHandler::MouseButton)event->button(), event->x(), event->y());
        });

        connect(this, &Renderer::mouseReleaseEvent, this, [this](QMouseEvent* event) {
            this->inputHandler->mouseRelease((InputHandler::MouseButton)event->button(), event->x(), event->y());
        });

        connect(this, &Renderer::wheelEvent, [this](QWheelEvent* event) {
            // this->inputHandler->mouseWheel(event->delta()); TODO
        });
        connect(this, &Renderer::keyPressEvent, this, [&](QKeyEvent* event) {
            this->inputHandler->keyPress(InputHandler::fromQtKey(event->key()));
        });

        connect(this, &Renderer::keyReleaseEvent, [this](QKeyEvent* event) {
            this->inputHandler->keyRelease(InputHandler::fromQtKey(event->key())); // TODO check why this is firing
        });
    }
    Renderer::~Renderer() {
    }
    void Renderer::initializeGL() {
        QSurfaceFormat surfaceFormat;
        surfaceFormat.setVersion(4, 6);
        surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
        surfaceFormat.setDepthBufferSize(8);
        context = new QOpenGLContext(this);
        context->setFormat(surfaceFormat);
        ge::gl::init();
        gl = std::make_shared<ge::gl::Context>();
        gl->glEnable(GL_DEPTH_TEST);
        gl->glEnable(GL_DEBUG_OUTPUT);
        gl->glDebugMessageCallback(MessageCallback, nullptr);

        SceneObject::init(gl);

        this->gBuffer = GBuffer(*gl);
        this->gBuffer.init(this->width(), this->height(), defaultFramebufferObject());

        QOpenGLWidget::initializeGL();

        frameStartTime = std::chrono::high_resolution_clock::now();
    }

    void Renderer::paintGL() {

        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEndTime - frameStartTime).count();
        frameStartTime = frameEndTime;
        lastFrameTime = frameTime / 1000000.0;
        SceneObject::setFrameTime(lastFrameTime);

        gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Go through all the passes and render them
        geometryPass();

        // Rendering is complete, update the screen
        update();

        QOpenGLWidget::paintGL();
    }

    void Renderer::resizeGL(int w, int h) {
        this->gBuffer.resize(w, h, defaultFramebufferObject());
        scene->setDimensions(w, h);
        QOpenGLWidget::resizeGL(w, h);
    }

    void Renderer::geometryPass() {
        // gBuffer.bindBuffer();

        gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

        scene->draw();
        gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    }

    void Renderer::setScene(std::shared_ptr<Scene> scene) {
        this->scene = scene;
        scene->setInputHandler(this->inputHandler);
        scene->setDimensions(width(), height());
        this->setFocus();
        this->options->setScene(scene);
    }

} // namespace MapGenerator::Renderer
