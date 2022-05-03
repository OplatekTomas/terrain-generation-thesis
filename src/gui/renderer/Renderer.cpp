//
// Created by tomas on 10.04.22.
//

#include "Renderer.h"
#include <iostream>
#include "config/ConfigReader.h"
#include "shaders_gui/Shaders.h"
#include <Logger.h>

bool Renderer::startGeneration(GeneratorOptions genOptions, std::string configPath) { //TODO remove static code
    bool hasError = false;
    auto config = ConfigReader::read(configPath, &hasError);
    if (hasError) {
        Logger::log("Error while reading config file. Nothing can render");
        return false;
    }
    this->generating = true;
    this->mapGenerator = std::make_shared<class MapGenerator>(config, genOptions);
    auto map = mapGenerator->generateMap();
    scene = std::make_shared<Scene3D>(map, gl, camera, gBuffer->getId());
    return true;
}


void Renderer::paintGL() {

    auto frameBuffer = (int) defaultFramebufferObject();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ssao->clear(frameBuffer);
    if (!generating) {
        update();
        QOpenGLWidget::paintGL();
        return;
    }
    if (scene == nullptr) {
        update();
        QOpenGLWidget::paintGL();
        return;
    }
    geometryPass();
    ssaoPass(frameBuffer);
    lightningPass();
    skyboxPass(frameBuffer);

    update();
    QOpenGLWidget::paintGL();
}

void Renderer::initializeGL() {
    surfaceFormat.setVersion(4, 5);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    surfaceFormat.setDepthBufferSize(8);
    context = new QOpenGLContext(this);
    context->setFormat(surfaceFormat);
    auto result = context->create();
    if (!result) {
        Logger::log("Error while creating OpenGL context");
    }
    ge::gl::init();
    gl = std::make_shared<ge::gl::Context>();
    gl->glEnable(GL_DEPTH_TEST);

    initializeCamera();
    initializeLightning();
    initializeGBuffer();
    initializeEffects();

    initialized = true;

    QOpenGLWidget::initializeGL();
}


void Renderer::ssaoPass(int frameBuffer) {
    if (!ssaoEnabled) {
      return;
    }
    ssao->render(gPosition, gNormal, frameBuffer);
    ssao->renderBlur(frameBuffer);
}

void Renderer::skyboxPass(int frameBuffer) {
    if (!skyboxEnabled) { return; }
    skybox->draw(frameBuffer);
}

void Renderer::geometryPass() {
    gl->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->getId());
    gl->glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const qreal retinaScale = devicePixelRatio();
    scene->setCulling(cullingEnabled, cullFactor);
    scene->draw(height(), width(), retinaScale);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
}


void Renderer::lightningPass() {
    const qreal scale = devicePixelRatio();
    gl->glViewport(0, 0, width() * scale, height() * scale);
    lightningProgram->use();
    lightningProgram->set3v("cameraPos", glm::value_ptr(camera->getPosition()));
    lightningProgram->setMatrix4fv("worldToView", glm::value_ptr(camera->getViewMatrix()));
    gPosition->bind(0);
    gNormal->bind(1);
    gAlbedo->bind(2);
    gSpecular->bind(3);
    ssao->bind(4);
    drawQuad();
    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->getId());
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFramebufferObject());
    gl->glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

}

void Renderer::drawQuad() {
    quadVAO->bind();
    gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Renderer::initializeEffects() {
    this->ssao = std::make_shared<SSAO>(this->gl, this->width(), this->height(), defaultFramebufferObject(),
                                        [this]() { drawQuad(); });
    checkGLError();
    this->skybox = std::make_shared<Skybox>(this->gl, this->camera, (float) width(), (float) height());
    checkGLError();
}

void Renderer::checkGLError() {
    GLenum error = gl->glGetError();
    if (error != GL_NO_ERROR) {
        Logger::log("OpenGL error: " + std::to_string(error));
    }
}

void Renderer::initializeGBufferTextures() {
    gBuffer->bind(GL_FRAMEBUFFER);
    //checkForErrors();

    int width = this->width();
    int height = this->height();
    gPosition = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA16F, 0, width, height);
    gl->glBindTexture(GL_TEXTURE_2D, gPosition->getId());
    gPosition->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gPosition->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition->getId(), 0);

    gNormal = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA16F, 0, width, height);
    gl->glBindTexture(GL_TEXTURE_2D, gNormal->getId());
    gNormal->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gNormal->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal->getId(), 0);

    gAlbedo = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA, 0, width, height);
    gl->glBindTexture(GL_TEXTURE_2D, gAlbedo->getId());
    gAlbedo->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gAlbedo->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo->getId(), 0);


    gSpecular = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA, 0, width, height);
    gl->glBindTexture(GL_TEXTURE_2D, gSpecular->getId());
    gSpecular->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gSpecular->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gSpecular->getId(), 0);


    unsigned int att[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    gl->glDrawBuffers(4, att);

    this->rboDepth = std::make_shared<ge::gl::Renderbuffer>();
    this->rboDepth->bind();
    gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth->getId());

    // finally check if framebuffer is complete
    if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::log("Framebuffer not complete. You should not see this message!!");
    gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

}

void Renderer::initializeCamera() {
    camera = std::make_shared<Camera>();
}

void Renderer::initializeLightning() {
    //Prepare the shaders for the lightning pass
    this->lightningVS = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, GUIShaders::getLightningVS());
    this->lightningFS = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, GUIShaders::getLightningFS());
    this->lightningProgram = std::make_shared<ge::gl::Program>(lightningVS, lightningFS);
}

void Renderer::initializeGBuffer() {
    //Prepare the quad that will get rendered
    float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };
    this->quadBuffer = std::make_shared<ge::gl::Buffer>(12 * sizeof(float), vertices);
    this->quadVAO = std::make_shared<ge::gl::VertexArray>();
    this->quadVAO->bind();
    quadVAO->addAttrib(quadBuffer, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    gBuffer = std::make_shared<ge::gl::Framebuffer>();
    initializeGBufferTextures();
}


void Renderer::resizeGL(int w, int h) {
    initializeGBufferTextures();
    ssao->setDimensions(w, h);
    skybox->setDimensions(w, h);
    QOpenGLWidget::resizeGL(w, h);
}

bool Renderer::event(QEvent *e) {
    switch (e->type()) {
        case QEvent::MouseMove:
            camera->mouseMoved(static_cast<QMouseEvent *>(e));
            return true;
        case QEvent::KeyPress:
            break;
        default:
            break;
    }
    return QOpenGLWidget::event(e);
}

void Renderer::keyPressEvent(QKeyEvent *event) {
    camera->keyEvent(event);
    QWidget::keyPressEvent(event);
}

void Renderer::keyReleaseEvent(QKeyEvent *event) {
    camera->keyEvent(event);
    QWidget::keyReleaseEvent(event);
}

void Renderer::scrollEvent(QWheelEvent *ev){
    camera->scrolled(ev);
}

void Renderer::cancelGeneration() {
    this->generating = false;
    this->mapGenerator->cancel();
    this->scene = nullptr;
    this->mapGenerator = nullptr;
}







