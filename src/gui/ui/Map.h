//
// Created by tomas on 12.04.22.
//


#pragma once

#include <marble/MarbleWidget.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

using namespace Marble;

class Map : public MarbleWidget {
Q_OBJECT
public:
    explicit Map(QWidget *parent = 0);
    void customPaint(GeoPainter* painter) override;

    glm::vec4 getSelectedArea();
    bool hasSelectedArea();
    ~Map() = default;

protected slots:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    bool isDragging = false;
    glm::vec2 dragStart;
    glm::vec2 dragEnd;
    glm::vec4 dragRect;

    void boundingBoxCreated(glm::vec2 start, glm::vec2 end);

    double getDistance(double lat1, double lat2, double lon1, double lon2);
};



