//
// Created by tomas on 06.10.21.
//

#include <iostream>
#include <renderer/Renderer.h>
#include <geGL/geGL.h>
#include <memory>
#include <thread>
#include <csignal>
#include "shaders/Shaders.h"
#include <config/ConfigReader.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QtConcurrent/QtConcurrent>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QGuiApplication>

namespace MapGenerator {
    Renderer::Renderer(QWindow *parent) {
        initialized = false;
        context = nullptr;
        setSurfaceType(
                QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
        surfaceFormat.setVersion(4, 5);
        surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
        surfaceFormat.setDepthBufferSize(8);
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

        auto currentPos = posBrno;

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
            camera = std::make_shared<Camera>((Renderer *) this);
            connect(this, SIGNAL(keyPressEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(keyReleaseEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(mouseMoveEvent(QMouseEvent * )), camera.get(), SLOT(mouseMoved(QMouseEvent * )));
        }
        //set up the context and get ready for rendering
        context->makeCurrent(this);
        ge::gl::init();
        gl = std::make_shared<ge::gl::Context>();
        gl->glEnable(GL_DEPTH_TEST);

        //start rendering loop
        renderTimer = std::make_unique<QTimer>(this);
        //grab the monitor refresh rate and set the timer to that
        auto screen = QGuiApplication::primaryScreen();
        refreshRate = 1000 / screen->refreshRate();
        renderTimer->setInterval((int)refreshRate);
        connect(renderTimer.get(), SIGNAL(timeout()), this, SLOT(renderNow()));
        renderTimer->start();
        initialized = true;
        //Grab the map and create the scene

        auto map = mapGenerator->generateMap();
        scene = std::make_shared<Scene3D>(map, gl, camera);
    }

    void Renderer::render() {
        const qreal retinaScale = devicePixelRatio();
        clearView();
        if (scene != nullptr) {
            //camera->print();
            scene->draw(height(), width(), retinaScale);
        }
        context->swapBuffers(this);

    }

    void Renderer::clearView() {
        const qreal retinaScale = devicePixelRatio();
        gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
        gl->glClearColor(0, 0, 0, 1.0);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Renderer::renderNow() {
        if (!isExposed()) return;
        if (!initialized) {
            initialize();
        };
        render();
    }


    bool Renderer::event(QEvent *event) {
        switch (event->type()) {
            case QEvent::UpdateRequest:
                renderNow();
                return true;
            case QEvent::Close:
                //TODO stop render thread
                //deleteLater();
                return QWindow::event(event);
            default:
                return QWindow::event(event);

        }
    }


    void Renderer::exposeEvent(QExposeEvent *event) {
        if (isExposed()) {
            renderNow();
        }
    }

}
