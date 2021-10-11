//
// Created by tomas on 06.10.21.
//

#include <iostream>
#include <renderer/Renderer.h>
#include <geGL/geGL.h>

#include <csignal>
#include <shaders/Shaders.h>
#include <ConfigReader.h>
#include <QCoreApplication>
#include <QMetaMethod>
#include <QKeyEvent>

Renderer::Renderer(QWindow *parent) {
    initialized = false;
    context = nullptr;
    setSurfaceType(QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
    surfaceFormat.setVersion(4, 5);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    setupLib();

}

bool Renderer::setupLib(){

    bool hasError = false;
    auto config = MapGenerator::ConfigReader::read(QCoreApplication::arguments().at(1).toStdString(), &hasError);
    if(hasError){
        return false;
    }
    this->mapGenerator = std::make_shared<MapGenerator::MapGenerator>(&config);
    return true;
}


void Renderer::initialize() {
    if (initialized) return;
    if (!context) {
        context = new QOpenGLContext(this);
        context->setFormat(surfaceFormat);
        bool success = context->create();
        if (!success) {
            //fail gracefully TODO:actually fail
            ::raise(SIGSEGV);
        }
        camera = std::make_shared<MapGenerator::Camera>();
        connect(this, SIGNAL(keyPressEvent(QKeyEvent*)), camera.get(), SLOT(keyPressEvent(QKeyEvent*)));
        connect(this, SIGNAL(keyReleaseEvent(QKeyEvent*)), camera.get(), SLOT(keyReleaseEvent(QKeyEvent*)));


    }

    //let's say to the OS that we want to work with this context
    context->makeCurrent(this);
    ge::gl::init();
    gl = std::make_shared<ge::gl::Context>();
    auto data = mapGenerator->getVertices();

    vertices = std::make_shared<ge::gl::Buffer>(data->vertices->size() * sizeof(float), data->vertices->data(), GL_STATIC_DRAW);
    indices = std::make_shared<ge::gl::Buffer>(data->indices->size() * sizeof(int), data->indices->data(), GL_STATIC_DRAW);

    vao = std::make_shared<ge::gl::VertexArray>();
    vao->addAttrib(vertices, 0, 3, GL_FLOAT);
    vao->addElementBuffer(indices);
    drawCount = data->indices->size();

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
    gl->glDrawElements(GL_TRIANGLES,drawCount,GL_UNSIGNED_INT, nullptr);

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



