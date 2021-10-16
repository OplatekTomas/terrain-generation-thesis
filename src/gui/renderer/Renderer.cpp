//
// Created by tomas on 06.10.21.
//

#include <iostream>
#include <renderer/Renderer.h>
#include <geGL/geGL.h>
#include <thread>

#include <csignal>
#include <shaders/Shaders.h>
#include <config/ConfigReader.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QtConcurrent/QtConcurrent>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MapGenerator{
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
        this->mapGenerator = std::make_shared<MapGenerator>(&config);
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
            camera = std::make_shared<Camera>((Renderer*)this);
            connect(this, SIGNAL(keyPressEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(keyReleaseEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(mouseMoveEvent(QMouseEvent *)), camera.get(), SLOT(mouseMoved(QMouseEvent * )));
        }

        //let's say to the OS that we want to work with this context
        context->makeCurrent(this);
        ge::gl::init();
        gl = std::make_shared<ge::gl::Context>();

        std::vector<double> posHome {
                49.886345859314645, 17.884317103291107,
                49.890439664583255, 17.870389844778817
                //49.89026759887128, 17.876138805985576
        };

        std::vector<double> posBrno = {
                49.210677743172724, 16.62863105170431,
                49.213095764793390, 16.625380048112635
        };

        std::vector<double> posHradec = {
                49.870291704376214, 17.860090558506485,
                49.888954501165955, 17.88689223413519,
        };
        auto draw = posHradec;

        auto data = mapGenerator->getVertices(draw[0], draw[1], draw[2], draw[3], 40);

        vertices = std::make_shared<ge::gl::Buffer>(data->vertices->size() * sizeof(float), data->vertices->data(),
                                                    GL_STATIC_DRAW);
        indices = std::make_shared<ge::gl::Buffer>(data->indices->size() * sizeof(int), data->indices->data(),
                                                   GL_STATIC_DRAW);

        vao = std::make_shared<ge::gl::VertexArray>();
        vao->addAttrib(vertices, 0, 3, GL_FLOAT);
        vao->addElementBuffer(indices);
        drawCount = data->indices->size();

        std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, VertexSource);
        std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,
                                                                                          FragmentSource);

        shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);

        initialized = true;
        //Draw as wireframe
        gl->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );


    }

    void Renderer::render() {
        const qreal retinaScale = devicePixelRatio();
        gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
        gl->glClearColor(0, 0, 0, 1.0);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto view = camera->getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)width() / (float)height(), 0.1f, 100.0f);

        shaderProgram->setMatrix4fv("view", glm::value_ptr(view));
        shaderProgram->setMatrix4fv("projection", glm::value_ptr(projection));

        shaderProgram->use();
        vao->bind();
        gl->glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, nullptr);
        context->swapBuffers(this);

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
