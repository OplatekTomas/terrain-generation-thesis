//
// Created by tomas on 06.10.21.
//

#include <iostream>
#include <renderer/Renderer.h>
#include <geGL/geGL.h>

#include <iostream>
#include <csignal>
#include <shaders/Shaders.h>

Renderer::Renderer(QWindow *parent) {
    initialized = false;
    context = nullptr;
    setSurfaceType(
            QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
    surfaceFormat.setVersion(4, 5);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
}

void Renderer::initialize() {
    if (initialized) return;
    //! [qt_context]
    if (!context) {
        context = new QOpenGLContext(this);
        context->setFormat(surfaceFormat);
        bool success = context->create();
        if (!success) {
            //fail gracefully TODO:actually fail
            ::raise(SIGSEGV);
        }
    }

    //let's say to the OS that we want to work with this context
    context->makeCurrent(this);
    ge::gl::init();
    gl = std::make_shared<ge::gl::Context>();
    //! [geGL_init]

    std::vector<float> trianglePos = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    std::vector<int> indices = {0,1,2};


    buffer = std::make_shared<ge::gl::Buffer>(trianglePos.size() * sizeof(float), trianglePos.data(), GL_STATIC_DRAW);
    indicesBuffer = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);


    vao = std::make_shared<ge::gl::VertexArray>();
    vao->addElementBuffer(indicesBuffer);
    vao->addAttrib(buffer, 0, 3, GL_FLOAT);


    std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, VertexSource);
    std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, FragmentSource);

    shaderProgram = std::make_shared<ge::gl::Program>(vertexShader,fragmentShader);

    initialized = true;
}

void Renderer::render() {
    const qreal retinaScale = devicePixelRatio();
    gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    gl->glClearColor(0, 0, 0, 1.0);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    shaderProgram->use();
    vao->bind();
    gl->glDrawArrays(GL_TRIANGLES,0,3);

    std::cout << "Render" << std::endl;
    context->swapBuffers(this);
}

void Renderer::renderNow() {
    if (!isExposed()) return;
    if (!initialized) initialize();
    render();
}

bool Renderer::event(QEvent *event) {

    switch (event->type()) {
        case QEvent::UpdateRequest:
            renderNow();
            return true;
        default:
            return QWindow::event(event);
    }
}

void Renderer::exposeEvent(QExposeEvent *event) {
    if (isExposed()) {
        renderNow();
    }
}

