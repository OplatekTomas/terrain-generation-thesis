//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevelopWindow.h" resolved

#include "ui_DevelopWindow.h"
#include <assets/shaders_gui/Shaders.h>
#include <memory>
#include <renderer/Renderer.h>
#include <renderer/scene_objects/Mesh.h>
#include <windows/DevelopWindow.h>

#include <QGridLayout>

using namespace MapGenerator;
using namespace MapGenerator::Renderer::SceneObjects;

DevelopWindow::DevelopWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::DevelopWindow) {
    ui->setupUi(this);

    auto layout = this->findChild<QGridLayout*>("baseGridLayout");

    auto renderer = new Renderer::Renderer(this);

    auto scene = createScene();
    renderer->setScene(scene);

    layout->addWidget(renderer);
}

std::shared_ptr<Renderer::Scene> DevelopWindow::createScene() {
    auto scene = std::make_shared<Renderer::Scene>();

    auto root = std::make_shared<Renderer::SceneObjects::Node>();
    root->setName("root");
    scene->setRoot(root);

    auto camera = std::make_shared<Renderer::SceneObjects::Camera>();
    camera->setName("camera");
    // camera->setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
    camera->acceptInput(true);
    camera->setMovementSpeed(0.01);
    root->setCamera(camera);

    auto quad = std::make_shared<Mesh>("quad");
    quad->addVertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0));
    quad->addVertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0));
    quad->addVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0));
    quad->addVertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0));

    quad->addVertex(glm::vec3(-1.0f, -1.0f, 5.0f), glm::vec3(0, 1.0, 0.0));
    quad->addVertex(glm::vec3(1.0f, -1.0f, 5.0f), glm::vec3(0, 1.0, 0.0));
    quad->addVertex(glm::vec3(1.0f, 1.0f, 5.0f), glm::vec3(0, 1.0, 0.0));
    quad->addVertex(glm::vec3(-1.0f, 1.0f, 5.0f), glm::vec3(0, 1.0, 0.0));

    quad->addVertex(glm::vec3(-1.0f, -1.0f, -5.0f), glm::vec3(0.0, 0.0, 1.0));
    quad->addVertex(glm::vec3(1.0f, -1.0f, -5.0f), glm::vec3(0.0, 0.0, 1.0));
    quad->addVertex(glm::vec3(1.0f, 1.0f, -5.0f), glm::vec3(0.0, 0.0, 1.0));
    quad->addVertex(glm::vec3(-1.0f, 1.0f, -5.0f), glm::vec3(0.0, 0.0, 1.0));

    quad->addIndex(0);
    quad->addIndex(1);
    quad->addIndex(2);
    quad->addIndex(0);
    quad->addIndex(2);
    quad->addIndex(3);

    quad->addIndex(4);
    quad->addIndex(5);
    quad->addIndex(6);
    quad->addIndex(4);
    quad->addIndex(6);
    quad->addIndex(7);

    quad->addIndex(8);
    quad->addIndex(9);
    quad->addIndex(10);
    quad->addIndex(8);
    quad->addIndex(10);
    quad->addIndex(11);

    root->setMesh(quad);

    auto program = std::make_shared<Program>("program");
    program->addShader(std::make_shared<Shader>(GUIShaders::getBasicVS(), "basic vertex", Shader::Type::VERTEX));
    program->addShader(std::make_shared<Shader>(GUIShaders::getBasicFS(), "basic fragment", Shader::Type::FRAGMENT));

    root->setProgram(program);

    scene->build();
    // scene
    return scene;
}

DevelopWindow::~DevelopWindow() {
    delete ui;
}
