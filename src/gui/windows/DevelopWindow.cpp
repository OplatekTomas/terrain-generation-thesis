//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevelopWindow.h" resolved

#include <windows/DevelopWindow.h>
#include "ui_DevelopWindow.h"
#include <renderer/Renderer.h>
#include <QGridLayout>

using namespace MapGenerator;

DevelopWindow::DevelopWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::DevelopWindow) {
    ui->setupUi(this);

    auto layout = this->findChild<QGridLayout *>("baseGridLayout");
    auto renderer = new Renderer::Renderer(this);
    layout->addWidget(renderer);

}

DevelopWindow::~DevelopWindow() {
    delete ui;
}
