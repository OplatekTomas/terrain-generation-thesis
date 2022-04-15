//
// Created by tomas on 15.04.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RendererLayout.h" resolved

#include <ui/rendererlayout.h>
#include "ui_rendererlayout.h"


RendererLayout::RendererLayout(QWidget *parent) : QWidget(parent), ui(new class Ui::RendererLayout) {
    ui->setupUi(this);
}

RendererLayout::~RendererLayout() {
    delete ui;
}

void RendererLayout::startRendering(glm::vec4 box, int terrainResolution) {
    GeneratorOptions options;
    options.lon1 = box.x;
    options.lat1 = box.y;
    options.lon2 = box.z;
    options.lat2 = box.w;
    options.terrainResolution = terrainResolution;
    auto renderer = ui->renderer;
    renderer->startGeneration(options, QCoreApplication::arguments().at(1).toStdString());
}

