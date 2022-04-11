
//
// Created by tomas on 02.10.21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <MainWindow.h>
#include <ui_MainWindow.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    baseGridLayout = this->findChild<QGridLayout *>("baseGridLayout");
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);

    auto glWidget = findChild<Renderer *>("openGLWidget");
    //register event filter
    installEventFilter(this);
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onRenderStarted() {
    auto glWidget = findChild<Renderer *>("openGLWidget");
    glWidget->startGeneration({}, {});
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        auto glWidget = findChild<Renderer *>("openGLWidget");
        glWidget->keyPressEvent(keyEvent);
    }
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        auto glWidget = findChild<Renderer *>("openGLWidget");
        glWidget->keyReleaseEvent(keyEvent);
    }
    return QObject::eventFilter(obj, event);

}

