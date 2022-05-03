//
// Created by tomas on 15.04.22.
//

#pragma once

#include <QWidget>
#include <glm/vec4.hpp>


QT_BEGIN_NAMESPACE
namespace Ui { class RendererLayout; }
QT_END_NAMESPACE

class RendererLayout : public QWidget {
Q_OBJECT

public:
    explicit RendererLayout(QWidget *parent = nullptr);

    void startRendering(glm::vec4 box, int terrainResolution = 64);

    ~RendererLayout() override;

protected slots:
    void backClicked();
    void ssaoChecked(int state);
    void skyboxChecked(int state);
    void cullingChecked(int state);
    void cullingDistanceChanged(int value);

private:
    Ui::RendererLayout *ui;
    bool backEnabled = false;

    void receivedMessage(const std::string &message);
};
