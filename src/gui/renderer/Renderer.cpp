//
// Created by tomas on 06.10.21.
//

#include <iostream>
#include <StaticFunctions.h>
#include <renderer/Renderer.h>
#include <geGL/geGL.h>
#include <memory>
#include <thread>
#include <csignal>
#include <config/ConfigReader.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QtConcurrent/QtConcurrent>
#include <glm/ext/matrix_transform.hpp>
#include <shaders_gui/Shaders.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QGuiApplication>
#include <QPainter>
#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>


namespace MapGenerator {
    Renderer::Renderer(QWindow *parent) : QWindow(parent), m_backingStore(new QBackingStore(this)) {
        initialized = false;
        context = nullptr;
        background = new BaseWidget();
        background->setFixedSize(size());
        background->setGeometry(0, 0, 300, 100);

        QPalette pal = QPalette();
// set black background
// Qt::black / "#000000" / "black"
        pal.setColor(QPalette::Window, Qt::white);
        background->setAutoFillBackground(true);
        background->setPalette(pal);

        QCoreApplication::instance()->installEventFilter(background);

        drawGui = true;
        swapUi();

        //setGeometry(100, 100, 300, 200);

        setupLib();

    }

    bool Renderer::setupLib() {
        bool hasError = false;
        //TODO: fix this - it can crash when there is no launch argument
        auto config = ConfigReader::read(QCoreApplication::arguments().at(1).toStdString(), &hasError);
        if (hasError) {
            return false;
        }
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
        this->mapGenerator = std::make_shared<MapGenerator>(config, options);
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
            //Set up the camera
            camera = std::make_shared<Camera>();
            connect(this, SIGNAL(keyPressEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(keyReleaseEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(mouseMoveEvent(QMouseEvent * )), camera.get(), SLOT(mouseMoved(QMouseEvent * )));
            connect(this, SIGNAL(wheelEvent(QWheelEvent * )), camera.get(), SLOT(scrolled(QWheelEvent * )));

        }
        //set up the context and get ready for renderin

        context->makeCurrent(this);
        ge::gl::init();
        gl = std::make_shared<ge::gl::Context>();
        gl->glEnable(GL_DEPTH_TEST);

        //start rendering loop
        renderTimer = std::make_unique<QTimer>(this);
        //grab the monitor refresh rate and set the timer to that
        auto screen = QGuiApplication::primaryScreen();
        refreshRate = 1000 / screen->refreshRate();
        renderTimer->setInterval((int) refreshRate);
        connect(renderTimer.get(), SIGNAL(timeout()), this, SLOT(renderNow()));
        renderTimer->start();
        initialized = true;
        //Grab the map and create the scene

        auto map = mapGenerator->generateMap();

        //Prepare gBuffer render targets
        initializeLightning();
        initializeGBuffer();
        this->ssao = std::make_shared<SSAO>(this->gl, this->width(), this->height(), 0 ,[this]() { drawQuad(); });
        this->skybox = std::make_shared<Skybox>(this->gl, this->camera, (float) width(), (float) height());
        //Prepare the scene - used in geometry pass
        scene = std::make_shared<Scene3D>(map, gl, camera, gBuffer->getId());
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
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void Renderer::checkForErrors() {
        GLenum err;
        while ((err = gl->glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
            throw std::runtime_error("OpenGL error");

        }
    }

    void Renderer::render(QPainter *painter) {
        background->render(painter);
        //button->show();
        //painter->drawText(QRectF(0, 0, width(), height()), Qt::AlignCenter, QStringLiteral("QWindow"));
    }

    void Renderer::render() {
        clearView();
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        geometryPass();
        ssao->render(gPosition, gNormal, 0);
        ssao->renderBlur(0);
        lightningPass();
        skybox->draw(0);
        context->swapBuffers(this);
    }

    void Renderer::geometryPass() {
        if (scene == nullptr) {
            return;
        }
        gl->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->getId());
        const qreal retinaScale = devicePixelRatio();
        scene->draw(height(), width(), retinaScale);
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    std::string Renderer::readShader(std::string path) {
        //Read entire file into string
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }


    void Renderer::lightningPass() {
        lightningProgram->use();
        gPosition->bind(0);
        gNormal->bind(1);
        gAlbedo->bind(2);
        ssao->bind(3);
        drawQuad();
        gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->getId());
        gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        gl->glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void Renderer::drawQuad() {
        quadVAO->bind();
        gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void Renderer::clearView() {
        const qreal retinaScale = devicePixelRatio();
        gl->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->getId());
        gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
        gl->glClearColor(0, 0, 0, 1.0);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Renderer::renderNow() {
        if (drawGui) {
            if (!isExposed())
                return;

            QRect rect(0, 0, width(), height());
            m_backingStore->beginPaint(rect);
            QPaintDevice *device = m_backingStore->paintDevice();
            QPainter painter(device);
            //painter.fillRect(0, 0, width(), height(), QGradient::NightFade);
            render(&painter);
            painter.end();


            m_backingStore->endPaint();
            m_backingStore->flush(rect);
        } else {
            if (!isExposed()) return;
            if (!initialized) {
                initialize();
            };
            render();
        }

    }

    void Renderer::resizeRender() {

        initializeGBufferTextures();
        ssao->setDimensions(this->width(), this->height());
        skybox->setDimensions(this->width(), this->height());
    }


    void Renderer::swapUi(){
        drawGui = !drawGui;
        if(drawGui){
            setSurfaceType(QWindow::RasterSurface);
        }else{
            setSurfaceType(
                    QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
            surfaceFormat.setVersion(4, 5);
            surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
            surfaceFormat.setDepthBufferSize(8);
        }
    }

    bool Renderer::event(QEvent *event) {
        //std::cout << event->type() << std::endl;
        switch (event->type()) {
            case QEvent::UpdateRequest:
                renderNow();
                return true;
            case QEvent::Resize:
                m_backingStore->resize(((QResizeEvent *) event)->size());

                if (initialized) {
                    resizeRender();
                }
                return true;
            case QEvent::Close:
                //TODO stop render thread
                //deleteLater();
                return QWindow::event(event);
            case QEvent::KeyPress:
                if (((QKeyEvent*)event)->key() == Qt::Key_Q) {
                    swapUi();
                    return true;
                }
                break;
            default:
                return QWindow::event(event);

        }
        return QWindow::event(event);
    }


    void Renderer::exposeEvent(QExposeEvent *event) {
        if (isExposed()) {
            renderNow();
        }
    }


}
