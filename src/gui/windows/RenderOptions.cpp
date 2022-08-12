
//
// Created by tomas on 02.10.21.
//

#include "StaticFunctions.h"
#include "internal/QCodeEditor.hpp"
#include "internal/QSyntaxStyle.hpp"
#include "ui_RenderOptions.h"
#include <QCodeEditor>
#include <QEvent>
#include <QGLSLCompleter>
#include <QGLSLHighlighter>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QSyntaxStyle>
#include <QWidget>
#include <memory>
#include <qgridlayout.h>
#include <qlayoutitem.h>
#include <views/controls/QDataTreeItem.h>

#include <QLabel>
#include <qnamespace.h>
#include <qtreewidget.h>
#include <windows/RenderOptions.h>

RenderOptions::RenderOptions(QWidget* parent) : QMainWindow(parent), ui(new Ui::RenderOptions) {
    ui->setupUi(this);

    this->nodeView = ui->nodeView;
    // ui->errorLabel->setText("No error");
    connect(nodeView, &QTreeWidget::itemSelectionChanged, this, &RenderOptions::onItemClicked);
    dataWidget = this->findChild<QGridLayout*>("dataWidget");
    //dataWidget->setSpacing(10);
    auto defaultText = new QLabel("No item selected");
    defaultText->setAlignment(Qt::AlignCenter);
    defaultText->setFixedWidth(500);
    dataWidget->layout()->addWidget(defaultText);
    // prepareCodeEditor();

    // baseGridLayout->addWidget(btn);
}

void RenderOptions::prepareCodeEditor() {
    codeEditor = new QCodeEditor(this);
    dataWidget->layout()->addWidget(codeEditor);
    codeEditor->setAutoIndentation(true);
    codeEditor->setAutoParentheses(true);
    codeEditor->setCompleter(new QGLSLCompleter(this));
    codeEditor->setHighlighter(new QGLSLHighlighter());
    codeEditor->setTabReplaceSize(4);

    auto qStyleString = QString(styleString.data());

    auto style = new QSyntaxStyle(this);
    codeEditor->setSyntaxStyle(style->load(qStyleString) ? style : QSyntaxStyle::defaultStyle());

    connect(codeEditor, &QCodeEditor::textChanged, this, &RenderOptions::codeChanged);
}

RenderOptions::~RenderOptions() {
    delete ui;
}

void RenderOptions::editShader(std::shared_ptr<Shader> shader) {
    prepareCodeEditor();
    currentShader = shader;
    codeEditor->setPlainText(shader->getSource().data());
}

void RenderOptions::onItemClicked() {
    auto widget = dataWidget->layout()->takeAt(0);
    if (widget) {
        delete widget->widget();
    }
    auto shader = dynamic_cast<QDataTreeItem<Shader>*>(nodeView->currentItem());
    if (shader != nullptr) {
        editShader(shader->getData());
        return;
    }
}

void RenderOptions::codeChanged() {
    auto code = codeEditor->toPlainText().toStdString();
    if (currentShader == nullptr) {
        return;
    }
    currentShader->setSource(code);
    currentShader->recompile();
    this->statusBar()->showMessage(currentShader->getInfoLog().data());
}

void RenderOptions::addNode(Node& node, QTreeWidgetItem* parent) {
    // TODO: Add more options to the debug window

    // Builds the base of the node
    auto root = parent ? new QTreeWidgetItem(parent) : new QTreeWidgetItem(nodeView);
    auto fullName = type(node);
    auto type = fullName.substr(fullName.find_last_of("::") + 1);
    root->setText(0, type.data());
    root->setText(1, node.getName().data());

    // Adds program information
    auto programNode = new QTreeWidgetItem(root);
    programNode->setText(0, "Program");
    auto program = node.getProgram();
    programNode->setText(1, program->getName().data());
    for (auto shader : program->getShaders()) {
        auto shaderNode = new QDataTreeItem<Shader>(programNode);
        shaderNode->setData(shader);
        shaderNode->setText(0, ("Shader (" + shader->getTypeString() + ")").data());
        shaderNode->setText(1, shader->getName().data());
    }

    //Adds the mesh

    auto meshNode = new QTreeWidgetItem(root);
    auto mesh = node.getMesh();
    meshNode->setText(0, "Mesh");
    meshNode->setText(1, mesh->getName().data());

    // Adds the children of the node
    auto childrenNode = new QTreeWidgetItem(root);
    childrenNode->setText(0, "Children");

    for (auto n : node.getChildren()) {
        addNode(*n, childrenNode);
    }
}

void RenderOptions::setScene(std::shared_ptr<MapGenerator::Renderer::Scene> scene) {
    this->scene = scene;
    addNode(*scene->getRoot(), nullptr);
};