//
// Created by tomas on 10.04.22.
//

#include "Renderer.h"
#include <iostream>
#include "config/ConfigReader.h"
#include "shaders_gui/Shaders.h"


void Renderer::startGeneration(GeneratorOptions genOptions, LibConfig libConf) { //TODO remove static code
    bool hasError = false;
    auto config = ConfigReader::read(QCoreApplication::arguments().at(1).toStdString(), &hasError);
    GeneratorOptions options;
    std::vector<double> posHome{
            49.883325913713, 17.8657865524292, 49.89402618295204, 17.890548706054688
    };

    std::vector<double> posHomeL{
            49.96736286729904, 17.860572894482512,

            49.8718795233479, 17.955027618972238};

    std::vector<double> posBrno = {
            49.19256141221154, 16.594543972568715,
            49.19827707820228, 16.604973078397315
    };

    std::vector<double> posRand = {
            49.9619918488622, 17.8586352852208, 49.91684506727818, 17.94300583538308

    };

    std::vector<double> posMountains = {
            50.10588121964279, 17.198770606455174,
            50.05709781257081, 17.28661015961763

    };
    std::vector<double> posBrnoVeryLarge{
            49.23019297366651, 16.565201713369547, 49.171611576900936, 16.71542469343281
    };


    std::vector<double> posBrnoMar{
            49.22264878390983, 16.695949499486055,
            49.20984129157041, 16.72238587076677
    };

    std::vector<double> brazil{
            -22.940051163948276, -43.226979529278665,
            -22.96603878773571, -43.18380161954447
    };

    std::vector<double> moni{
            49.206206330276416, 16.693695548101253,
            49.20443981462129, 16.697611471199462
    };

    std::vector<double> nassfeld{
            46.631426377462304, 13.222294893455746,
            46.55290962338361, 13.297562841791274
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

void Renderer::paintGL() {
    auto frameBuffer = (int) defaultFramebufferObject();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
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

void Renderer::initializeGL() {
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


void Renderer::geometryPass() {
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


void Renderer::lightningPass() {
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

void Renderer::checkGLError(){
    GLenum error = gl->glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
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



