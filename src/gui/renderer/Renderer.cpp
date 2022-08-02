//
// Created by tomas on 2.8.22.
//

#include <StaticFunctions.h>
#include <renderer/Renderer.h>

namespace MapGenerator::Renderer {
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

        this->gBuffer = GBuffer(*gl);
        this->gBuffer.init(this->height(), this->width(), defaultFramebufferObject());

        QOpenGLWidget::initializeGL();
    
    }

    void Renderer::paintGL() {
        gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl->glViewport(0, 0, this->width(), this->height());

        // Go through all the passes and render them
        geometryPass();

        // Rendering is complete, update the screen
        update();

        QOpenGLWidget::paintGL();
    }

    void Renderer::resizeGL(int w, int h) {
        this->gBuffer.resize(h, w, defaultFramebufferObject());
        QOpenGLWidget::resizeGL(w, h);
    }

    void Renderer::geometryPass() {
        gBuffer.bindBuffer();

        gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    }

} // namespace MapGenerator::Renderer
