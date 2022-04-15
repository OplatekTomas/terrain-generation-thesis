
//
// Created by tomas on 02.10.21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <MainWindow.h>
#include <ui_MainWindow.h>
#include <ui/mapview.h>
#include <ui/rendererlayout.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    isRendering = false;
    ui->setupUi(this);
    baseGridLayout = this->findChild<QGridLayout *>("baseGridLayout");
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    installEventFilter(this);
    auto mapView = new MapView(this);
    mapView->setObjectName("mapView");
    mapView->setSizePolicy(sizePolicy);

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

void MainWindow::drawMap(glm::vec4 box) {
    auto mapWidget = findChild<MapView *>("mapView");
    mapWidget->setVisible(false);
    rendererLayout->setVisible(true);
    ui->baseGridLayout->removeWidget(mapWidget);
    ui->baseGridLayout->addWidget(rendererLayout);
    //create renderer
    //auto tmp = glm::vec4( 49.883325913713, 17.8657865524292, 49.89402618295204, 17.890548706054688);
    rendererLayout->startRendering(box);
    this->isRendering = true;
}

