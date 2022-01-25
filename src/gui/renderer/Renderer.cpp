//
// Created by tomas on 06.10.21.
//

#include <iostream>
#include <renderer/Renderer.h>
#include <geGL/geGL.h>
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

namespace MapGenerator {
    Renderer::Renderer(QWindow *parent) {
        initialized = false;
        context = nullptr;
        setSurfaceType(
                QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
        surfaceFormat.setVersion(4, 5);
        surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
        setupLib();

    }

    bool Renderer::setupLib() {

        bool hasError = false;
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

        auto currentPos = posHome;

        options.lat1 = currentPos[0];
        options.lon1 = currentPos[1];
        options.lat2 = currentPos[2];
        options.lon2 = currentPos[3];
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
            camera = std::make_shared<Camera>((Renderer *) this);
            connect(this, SIGNAL(keyPressEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(keyReleaseEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(mouseMoveEvent(QMouseEvent * )), camera.get(), SLOT(mouseMoved(QMouseEvent * )));
        }

        //let's say to the OS that we want to work with this context
        context->makeCurrent(this);
        ge::gl::init();
        gl = std::make_shared<ge::gl::Context>();


        clearView();
        context->swapBuffers(this);


        auto map = mapGenerator->generateMap();
        scene = std::make_shared<Scene3D>(map, gl, camera);
        initialized = true;
    }

    void Renderer::render() {
        clearView();

        if (scene != nullptr) {
            //draw as wireframe
            gl->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            scene->draw(height(), width());
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
                if (watcher.isRunning()) {
                    watcher.future().cancel();
                }
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
