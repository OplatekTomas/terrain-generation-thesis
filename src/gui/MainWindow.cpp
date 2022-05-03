
//
// Created by tomas on 02.10.21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <MainWindow.h>
#include <ui_MainWindow.h>
#include <Logger.h>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    Logger::init(true);
    isRendering = false;
    ui->setupUi(this);
    baseGridLayout = this->findChild<QGridLayout *>("baseGridLayout");
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    installEventFilter(this);

    this->mapView = new MapView(this);
    this->mapView->setObjectName("mapView");
    this->mapView->setSizePolicy(sizePolicy);
    baseGridLayout->addWidget(mapView);

    rendererLayout = new RendererLayout(this);
    rendererLayout->setVisible(false);
    rendererLayout->setObjectName("rendererLayout");
    rendererLayout->setSizePolicy(sizePolicy);
    //baseGridLayout->addWidget(rendererLayout);

    auto btn = new QPushButton("Test");
    btn->setObjectName("testButton");
    connect(btn, &QPushButton::clicked, this, [&]{
        glm::vec4 box = {49.23757036758039, 16.498296596199317, 49.159263117191315, 16.676375765580808};
        box = glm::vec4(box.y, box.x, box.w, box.z);
        drawRenderer(box);
    });
    //baseGridLayout->addWidget(btn);
}


MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (!isRendering) {
        return QObject::eventFilter(obj, event);
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        auto glWidget = findChild<Renderer *>("renderer");
        if(glWidget != nullptr) {
            glWidget->keyPressEvent(keyEvent);
        }
        glWidget->keyPressEvent(keyEvent);
    }
    if (event->type() == QEvent::KeyRelease) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        auto glWidget = findChild<Renderer *>("renderer");
        if(glWidget == nullptr) {
            return QObject::eventFilter(obj, event);
        }
        glWidget->keyReleaseEvent(keyEvent);
    }
    if(event->type() == QEvent::Wheel) {
        auto *wheelEvent = static_cast<QWheelEvent *>(event);
        auto glWidget = findChild<Renderer *>("renderer");
        if(glWidget == nullptr) {
            return QObject::eventFilter(obj, event);
        }
        glWidget->scrollEvent(wheelEvent);
    }
    return QObject::eventFilter(obj, event);

}

void MainWindow::drawRenderer(glm::vec4 box) {
    mapView->setVisible(false);
    rendererLayout->setVisible(true);
    ui->baseGridLayout->removeWidget(mapView);
    ui->baseGridLayout->addWidget(rendererLayout);
    rendererLayout->startRendering(box);
    this->isRendering = true;
}

void MainWindow::drawMap(){
    mapView->setVisible(true);
    rendererLayout->setVisible(false);
    ui->baseGridLayout->removeWidget(rendererLayout);
    rendererLayout->deleteLater();
    rendererLayout = new RendererLayout(this);
    ui->baseGridLayout->addWidget(mapView);
    this->isRendering = false;
}

