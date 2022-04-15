//
// Created by tomas on 12.04.22.
//

#include <ui/Map.h>
#include <marble/GeoPainter.h>
#include <QApplication>
#include <marble/ViewportParams.h>
#include <marble/MarbleWidget.h>
#include <marble/GeoDataLatLonAltBox.h>
#include <marble/MarbleWidgetInputHandler.h>

#


#include <iostream>

Map::Map(QWidget *parent) : MarbleWidget(parent) {
    this->dragRect = glm::vec4(0);
    setProjection(Marble::Mercator);
    setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    setShowBorders(true);
    setShowCompass(false);
    setShowOverviewMap(false);
    setShowScaleBar(false);
    setShowCrosshairs(false);
    setShowGrid(false);
    setStyleSheet("background-color: black;");

    //Get marble presenter

}


void Map::customPaint(GeoPainter *painter) {
    auto degreeEnum = Marble::GeoDataCoordinates::Degree;
    //Calculate center of rectangle
    auto selectedCenterLon = (dragRect.x + dragRect.z) / 2;
    auto selectedCenterLat = (dragRect.y + dragRect.w) / 2;
    GeoDataCoordinates mid(selectedCenterLon, selectedCenterLat, 0.0, degreeEnum);
    //Calculate size of rectangle
    auto pixelsPerDegreeWidth = viewport()->size().width() / viewport()->viewLatLonAltBox().width(degreeEnum);
    auto pixelsPerDegreeHeight = viewport()->size().height() / viewport()->viewLatLonAltBox().height(degreeEnum);
    auto selectedWidth = std::abs(dragRect.x - dragRect.z) * pixelsPerDegreeWidth;
    auto selectedHeight = std::abs(dragRect.y - dragRect.w) * pixelsPerDegreeHeight;
    //Draw rectangle
    painter->setPen(QPen(Qt::red, 3));
    painter->setBrush(QBrush(QColor(200, 0, 0, 80)));
    painter->drawRect(mid, selectedWidth, selectedHeight);
    MarbleWidget::customPaint(painter);
}

void Map::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isDragging = true;
        dragStart = glm::vec2(event->localPos().x(), event->localPos().y());
    }
    QWidget::mousePressEvent(event);
}

void Map::mouseReleaseEvent(QMouseEvent *event) {
    if (isDragging && event->button() == Qt::RightButton) {
        isDragging = false;
        dragEnd = glm::vec2(event->localPos().x(), event->localPos().y());
        boundingBoxCreated(dragStart, dragEnd);
    }
    QWidget::mouseReleaseEvent(event);
}

void Map::boundingBoxCreated(glm::vec2 start, glm::vec2 end) {
    double startX, startY = 0;
    viewport()->geoCoordinates(start.x, start.y, startX, startY, Marble::GeoDataCoordinates::Degree);
    double endX, endY = 0;
    viewport()->geoCoordinates(end.x, end.y, endX, endY, Marble::GeoDataCoordinates::Degree);
    dragRect = glm::vec4(endX, endY, startX, startY);
    update();
}

glm::vec4 Map::getSelectedArea() {
    return dragRect;
}

bool Map::hasSelectedArea() {
    return dragRect.x != 0 && dragRect.y != 0 && dragRect.z != 0 && dragRect.w != 0;
}



