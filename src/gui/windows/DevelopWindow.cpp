//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevelopWindow.h" resolved

#include "renderer/scene_objects/Texture.h"
#include "renderer/scene_objects/Uniform.h"
#include "ui_DevelopWindow.h"
#include <MeshGenerators.h>
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

void DevelopWindow::createSpheres(std::shared_ptr<Renderer::SceneObjects::Node>& root, std::shared_ptr<Camera>& camera) {
    auto count = 4;

    auto sphereProgram = std::make_shared<Program>("sphere program");
    sphereProgram->addShader(std::make_shared<Shader>("basic vertex", GUIShaders::getBasicVS(), Shader::Type::VERTEX));
    sphereProgram->addShader(std::make_shared<Shader>("basic fragment", GUIShaders::getBasicFS(), Shader::Type::FRAGMENT));

    auto sphere = MeshGenerators::createIcoSphere(1.0f, 4, "icoSphere");

    for (int x = 0; x < count; x++) {
        for (int y = 0; y < count; y++) {
            auto sphereNode = std::make_shared<Node>("sphere (" + std::to_string(x) + ", " + std::to_string(y) + ")");
            sphereNode->setMesh(sphere);
            sphereNode->setProgram(sphereProgram);
            sphereNode->addUniform(std::make_shared<Uniform>("position", glm::vec2(x * 5, y * 5), Uniform::VEC2));
            sphereNode->setCamera(camera);
            root->addChild(sphereNode);
        }
    }
}

std::shared_ptr<Renderer::Scene> DevelopWindow::createScene() {
    auto scene = std::make_shared<Renderer::Scene>();

    auto camera = std::make_shared<Renderer::SceneObjects::Camera>("camera");
    camera->acceptInput(true);
    camera->setMovementSpeed(0.01);
    camera->setPosition(glm::vec3(0, 0.5, 0));

    auto plane = std::make_shared<Node>("root - plane");

    auto planeVerexShader = std::make_shared<Shader>(GUIShaders::getPlaneVS(), Shader::Type::VERTEX);
    auto planeFragmentShader = std::make_shared<Shader>(GUIShaders::getPlaneFS(), Shader::Type::FRAGMENT);
    auto planeProgram = std::make_shared<Program>("plane program", planeVerexShader, planeFragmentShader);

    auto texture = std::make_shared<Texture>("texture");

    plane->setMesh(MeshGenerators::createPlane(10, 10, "plane"));
    plane->setProgram(planeProgram);
    plane->setCamera(camera);

    scene->setRoot(plane);

    scene->build();
    // scene
    return scene;
}

DevelopWindow::~DevelopWindow() {
    delete ui;
}
