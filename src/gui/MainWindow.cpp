//
// Created by tomas on 02.10.21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <MainWindow.h>
#include <ui_MainWindow.h>


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    baseGridLayout = this->findChild<QGridLayout*>("baseGridLayout");
    //this->renderer = new MapRenderer::Renderer();
    //baseGridLayout->addWidget(renderer);
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);

}

MainWindow::~MainWindow() {
    delete ui;
}

