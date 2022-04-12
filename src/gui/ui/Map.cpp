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
    auto pixelsPerDegree = viewport()->size().width() / viewport()->viewLatLonAltBox().width(degreeEnum);
    auto selectedWidth = std::abs(dragRect.x - dragRect.z) * pixelsPerDegree;
    auto selectedHeight = std::abs(dragRect.y - dragRect.w) * pixelsPerDegree;
    //Draw rectangle
    painter->setPen(Qt::red);
    painter->setBackground(Qt::darkRed);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->drawRect(mid, selectedWidth, selectedHeight);

    std::cout << "Mid: " << selectedCenterLon << " " << selectedCenterLat << std::endl;
    std::cout << "Size: " << selectedWidth << " " << selectedHeight << std::endl;
    std::cout << "BBox: " << dragRect.x << " " << dragRect.y << " " << dragRect.z << " " << dragRect.w << std::endl;
    MarbleWidget::customPaint(painter);

}

void Map::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isDragging = true;
        dragStart = glm::vec2(event->localPos().x(), event->localPos().y());
        std::cout << "Ev: " << event->localPos().x() << " " << event->localPos().y() << std::endl;

    }
    QWidget::mousePressEvent(event);
}

void Map::mouseReleaseEvent(QMouseEvent *event) {
    if (isDragging && event->button() == Qt::RightButton) {
        isDragging = false;
        dragEnd = glm::vec2(event->localPos().x(), event->localPos().y());
        boundingBoxCreated(dragStart, dragEnd);
        std::cout << "Ev: " << event->localPos().x() << " " << event->localPos().y() << std::endl;
    }
    QWidget::mouseReleaseEvent(event);
}

void Map::boundingBoxCreated(glm::vec2 start, glm::vec2 end) {
    auto degreeEnum = Marble::GeoDataCoordinates::Degree;
    //Calculate number of pixels per degree
    auto pixelsPerDegree = viewport()->size().width() / viewport()->viewLatLonAltBox().width(degreeEnum);
    //Calculate bounding box
    auto startLat = start.y / pixelsPerDegree;
    auto startLon = start.x / pixelsPerDegree;
    auto endLat = end.y / pixelsPerDegree;
    auto endLon = end.x / pixelsPerDegree;
    //Calculate latitude and longitude position in the world
    auto startLatWorld = viewport()->viewLatLonAltBox().center().latitude(degreeEnum) - startLat;
    auto startLonWorld = viewport()->viewLatLonAltBox().center().longitude(degreeEnum) - startLon;
    auto endLatWorld = viewport()->viewLatLonAltBox().center().latitude(degreeEnum) - endLat;
    auto endLonWorld = viewport()->viewLatLonAltBox().center().longitude(degreeEnum) - endLon;
    std::cout << "Start: " << startLat << " " << startLon << std::endl;
    std::cout << "End: " << endLat << " " << endLon << std::endl;
    //Create bounding box
    dragRect = glm::vec4(startLonWorld, startLatWorld, endLonWorld, endLatWorld);
    //Create bounding box

}



