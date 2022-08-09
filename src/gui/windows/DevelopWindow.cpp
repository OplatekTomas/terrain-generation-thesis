//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevelopWindow.h" resolved

#include "ui_DevelopWindow.h"
#include <memory>
#include <renderer/Renderer.h>
#include <renderer/scene_objects/Mesh.h>
#include <windows/DevelopWindow.h>

#include <QGridLayout>

using namespace MapGenerator;
using namespace MapGenerator::Renderer::SceneObjects;

DevelopWindow::DevelopWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DevelopWindow) {
    ui->setupUi(this);

    auto layout = this->findChild<QGridLayout *>("baseGridLayout");

    auto renderer = new Renderer::Renderer(this);
    
    auto scene = createScene();
    renderer->setScene(scene);

    layout->addWidget(renderer);

}

std::shared_ptr<Renderer::Scene> DevelopWindow::createScene(){
    auto scene = std::make_shared<Renderer::Scene>();

    auto cube = std::make_shared<Mesh>();


    //scene
    return scene;
}

DevelopWindow::~DevelopWindow() {
    delete ui;
}
