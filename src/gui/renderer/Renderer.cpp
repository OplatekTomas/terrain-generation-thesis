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
            camera = std::make_shared<Camera>((Renderer *) this);
            connect(this, SIGNAL(keyPressEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(keyReleaseEvent(QKeyEvent * )), camera.get(), SLOT(keyEvent(QKeyEvent * )));
            connect(this, SIGNAL(mouseMoveEvent(QMouseEvent * )), camera.get(), SLOT(mouseMoved(QMouseEvent * )));
            connect(&watcher, &QFutureWatcher<std::tuple<std::shared_ptr<std::vector<float>>, int>>::finished, this, &Renderer::handleFinished);

        }

        //let's say to the OS that we want to work with this context
        context->makeCurrent(this);
        ge::gl::init();
        gl = std::make_shared<ge::gl::Context>();

        const qreal retinaScale = devicePixelRatio();

        gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
        gl->glClearColor(0.0, 0, 0, 1.0);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        context->swapBuffers(this);

        std::vector<double> posHome{
                49.883325913713, 17.8657865524292, 49.89402618295204, 17.890548706054688
        };

        std::vector<double> posBrno = {
                49.19256141221154, 16.594543972568715,
                49.19827707820228, 16.604973078397315
        };

        std::vector<double> posRand = {
                49.20322968539299, 16.60926962473714, 49.201543696771644, 16.61236590263251

        };

        std::vector<double> posMountains = {
                50.10588121964279, 17.198770606455174,
                50.05709781257081, 17.28661015961763

        };
        drawArea = posHome;
        //Getting the mesh
        auto data = mapGenerator->getVertices(drawArea[0], drawArea[1], drawArea[2], drawArea[3], 30);
        vertices = std::make_shared<ge::gl::Buffer>(data->vertices->size() * sizeof(float), data->vertices->data(),
                                                    GL_STATIC_DRAW);
        indices = std::make_shared<ge::gl::Buffer>(data->indices->size() * sizeof(int), data->indices->data(),
                                                   GL_STATIC_DRAW);
        vao = std::make_shared<ge::gl::VertexArray>();
        vao->addAttrib(vertices, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
        vao->addAttrib(vertices, 1, 2, GL_FLOAT, 5 * sizeof(float), 3 * sizeof(float));
        vao->addElementBuffer(indices);
        drawCount = data->indices->size();

        //Getting the texture
        startTextureGeneration(drawArea, 8);
        //auto texData = mapGenerator->getMetadata(draw[0], draw[1], draw[2], draw[3], resolution);
        //createTexture(*texData, resolution, resolution);

        auto vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, VertexSource);
        auto fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,
                                                               FragmentSource);
        shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);
        initialized = true;
        //Draw as wireframe
        //gl->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    }

    void Renderer::startTextureGeneration(const std::vector<double> &draw, const int& resolution) {
        auto generator = std::shared_ptr<MapGenerator>(mapGenerator);
        auto future = QtConcurrent::run([draw, resolution, generator]() {
            return std::make_tuple(generator->getMetadata(draw[0], draw[1], draw[2], draw[3], resolution), resolution);
        });
        watcher.setFuture(future);
    }

    void Renderer::handleFinished() {
        auto res = watcher.result();
        auto data = std::get<0>(res);
        auto resolution= std::get<1>(res);
        createTexture(*data, resolution, resolution);
        std::cout << "Finished drawing: " << resolution << std::endl;
        renderNow();
        startTextureGeneration(drawArea, resolution * 2);
        //watcher.disconnect();
    }

    void Renderer::createTexture(const std::vector<float> &data, int width, int height) {
        texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA32F, 0, width, height);
        texture->bind(GL_TEXTURE_2D);
        texture->setData2D(data.data(), GL_RGBA, GL_FLOAT, 0, GL_TEXTURE_2D, 0, 0, width, height);
        //texture->generateMipmap();
        texture->texParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
        texture->texParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
        texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        texture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        texture->bind(0);

    }

    void Renderer::render() {
        const qreal retinaScale = devicePixelRatio();
        gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
        gl->glClearColor(0.0, 0, 0, 1.0);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto view = camera->getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float) width() / (float) height(), 0.005f,
                                                100.0f);

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
