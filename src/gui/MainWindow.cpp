
//
// Created by tomas on 02.10.21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <MainWindow.h>
#include <ui_MainWindow.h>
#include <Logger.h>


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

    // Add button to create the map

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
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        auto glWidget = findChild<Renderer *>("renderer");
        if(glWidget == nullptr) {
            return QObject::eventFilter(obj, event);
        }
        glWidget->keyReleaseEvent(keyEvent);
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

