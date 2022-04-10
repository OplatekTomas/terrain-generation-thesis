//
// Created by tomas on 10.04.22.
//

#include <glwrapper.h>
#include <iostream>
#include <config/ConfigReader.h>
#include <shaders_gui/Shaders.h>


void GLWrapper::paintGL() {
    auto frameBuffer = (int) defaultFramebufferObject();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    geometryPass();
    ssao->render(gPosition, gNormal, frameBuffer);
    ssao->renderBlur(frameBuffer);
    lightningPass();
    skybox->draw(frameBuffer);
    update();
    QOpenGLWidget::paintGL();
}

void GLWrapper::initializeGL() {
    surfaceFormat.setVersion(4, 5);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    surfaceFormat.setDepthBufferSize(8);
    context = new QOpenGLContext(this);
    context->setFormat(surfaceFormat);
    auto result = context->create();
    if (!result) {
        std::cout << "Failed to create OpenGL context" << std::endl;
        exit(1); //TODO FAIL
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


void GLWrapper::geometryPass() {
    if (scene == nullptr) {
        return;
    }
    gl->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->getId());
    gl->glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const qreal retinaScale = devicePixelRatio();
    scene->draw(height(), width(), retinaScale);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
}


void GLWrapper::lightningPass() {
    lightningProgram->use();
    gPosition->bind(0);
    gNormal->bind(1);
    gAlbedo->bind(2);
    ssao->bind(3);
    drawQuad();
    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->getId());
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFramebufferObject());
    gl->glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

}

void GLWrapper::drawQuad() {
    quadVAO->bind();
    gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void GLWrapper::initializeEffects() {
    this->ssao = std::make_shared<SSAO>(this->gl, this->width(), this->height(), defaultFramebufferObject(),
                                        [this]() { drawQuad(); });
    this->skybox = std::make_shared<Skybox>(this->gl, this->camera, (float) width(), (float) height());
}


void GLWrapper::initializeGBufferTextures() {
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

    unsigned int att[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    gl->glDrawBuffers(3, att);

    this->rboDepth = std::make_shared<ge::gl::Renderbuffer>();
    this->rboDepth->bind();
    gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth->getId());

    // finally check if framebuffer is complete
    if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

}

void GLWrapper::initializeCamera() {
    camera = std::make_shared<Camera>();
}

void GLWrapper::initializeLightning() {
    //Prepare the shaders for the lightning pass
    this->lightningVS = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, GUIShaders::getLightningVS());
    this->lightningFS = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, GUIShaders::getLightningFS());
    this->lightningProgram = std::make_shared<ge::gl::Program>(lightningVS, lightningFS);
}

void GLWrapper::initializeGBuffer() {
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

void GLWrapper::startGeneration(GeneratorOptions genOptions, LibConfig libConf) { //TODO remove static code
    bool hasError = false;
    auto config = ConfigReader::read(QCoreApplication::arguments().at(1).toStdString(), &hasError);
    GeneratorOptions options;
    std::vector<double> posHome{
            49.883325913713, 17.8657865524292, 49.89402618295204, 17.890548706054688
    };
    auto currentPos = posHome;

    options.lat1 = currentPos[0];
    options.lon1 = currentPos[1];
    options.lat2 = currentPos[2];
    options.lon2 = currentPos[3];
    options.terrainResolution = 32;
    this->mapGenerator = std::make_shared<class MapGenerator>(config, options);
    auto map = mapGenerator->generateMap();
    scene = std::make_shared<Scene3D>(map, gl, camera, gBuffer->getId());

}


void GLWrapper::resizeGL(int w, int h) {
    initializeGBufferTextures();
    ssao->setDimensions(w, h);
    skybox->setDimensions(w, h);
    QOpenGLWidget::resizeGL(w, h);
}

bool GLWrapper::event(QEvent *e) {
    switch (e->type()) {
        case QEvent::MouseMove:
            camera->mouseMoved(static_cast<QMouseEvent *>(e));
            return true;
        case QEvent::KeyPress:
            std::cout << "Key pressed" << std::endl;
            break;
        default:
            break;
    }
    return QOpenGLWidget::event(e);
}

void GLWrapper::keyPressEvent(QKeyEvent *event) {
    camera->keyEvent(event);
    QWidget::keyPressEvent(event);
}

void GLWrapper::keyReleaseEvent(QKeyEvent *event) {
    camera->keyEvent(event);
    QWidget::keyReleaseEvent(event);
}


