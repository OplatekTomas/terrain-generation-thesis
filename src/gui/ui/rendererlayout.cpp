//
// Created by tomas on 15.04.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RendererLayout.h" resolved

#include <ui/rendererlayout.h>
#include <QMessageBox>
#include "ui_rendererlayout.h"
#include <MainWindow.h>
#include <Logger.h>

RendererLayout::RendererLayout(QWidget *parent) : QWidget(parent), ui(new class Ui::RendererLayout) {
    ui->setupUi(this);
    ui->log->setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);
    Logger::setTargetFn([this](const std::string &msg) {
        receivedMessage(msg);
    });
}

RendererLayout::~RendererLayout() {
    delete ui;
}

void RendererLayout::receivedMessage(const std::string &message) {
    if (ui->log == nullptr) {
        return;
    }
    auto *timer = new QTimer();
    timer->moveToThread(qApp->thread());
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, [=]() {
        // main thread
        ui->log->append("- " + QString::fromStdString(message));
        timer->deleteLater();
    });
    QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection, Q_ARG(int, 0));

}

void RendererLayout::startRendering(glm::vec4 box) {
    auto renderer = ui->renderer;
    renderer->startGeneration({box.y, box.w},
                              {box.x, box.z},
                              QCoreApplication::arguments().at(1).toStdString());
}

void RendererLayout::backClicked() {
    Logger::setTargetFn([this](const std::string &msg) {
    });
    if (!ui->renderer->canCancel()) {
        QMessageBox::warning(this, "Please wait",
                             "Please wait for download of metadata to finish.\nUntil then, you can't go back.");
        return;
    }
    ui->renderer->cancelGeneration();
    auto mainWindow = (MainWindow *) qApp->activeWindow();
    if (mainWindow == nullptr) {
        QMessageBox::warning(this, "No window", "No window is active - this should not happen");
        return;
    }
    mainWindow->drawMap();
}

void RendererLayout::cullingChecked(int state) {
    ui->renderer->cullingEnabled = state == Qt::Checked;
}

void RendererLayout::skyboxChecked(int state) {
    ui->renderer->skyboxEnabled = state == Qt::Checked;
}


void RendererLayout::ssaoChecked(int state) {
    ui->renderer->ssaoEnabled = state == Qt::Checked;
}

void RendererLayout::cullingDistanceChanged(int value) {
    ui->renderer->cullFactor = -(value / 100.0f);
}

