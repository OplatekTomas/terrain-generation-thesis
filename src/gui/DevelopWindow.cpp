//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DevelopWindow.h" resolved

#include "DevelopWindow.h"
#include "ui_DevelopWindow.h"


DevelopWindow::DevelopWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::DevelopWindow) {
    ui->setupUi(this);
}

DevelopWindow::~DevelopWindow() {
    delete ui;
}
