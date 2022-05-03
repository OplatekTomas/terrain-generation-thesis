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
#include <iostream>
#include <QMessageBox>

Map::Map(QWidget *parent) : MarbleWidget(parent) {
    this->dragRect = glm::vec4(0);
    setProjection(Marble::Mercator);
    //Use satellite imagery
    setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    setShowBorders(true);
    setShowCompass(false);
    setShowOverviewMap(false);
    setShowScaleBar(true);
    setShowCrosshairs(false);
    setShowGrid(false);
    setStyleSheet("background-color: black;");
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

void Map::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        double endX, endY = 0;
        double startX, startY = 0;
        viewport()->geoCoordinates(dragStart.x, dragStart.y, startX, startY, Marble::GeoDataCoordinates::Degree);
        viewport()->geoCoordinates(event->localPos().x(), event->localPos().y(), endX, endY,
                                   Marble::GeoDataCoordinates::Degree);
        dragRect = glm::vec4(endX, endY, startX, startY);
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void Map::mouseReleaseEvent(QMouseEvent *event) {
    if (isDragging && event->button() == Qt::RightButton) {
        isDragging = false;
        dragEnd = glm::vec2(event->localPos().x(), event->localPos().y());
        boundingBoxCreated(dragStart, dragEnd);
    }
    QWidget::mouseReleaseEvent(event);
}

double Map::getDistance(double lat1, double lat2, double long1, double long2) {
    const auto R = 6378.137; // Radius of earth in KM
    auto dLat = lat2 * M_PI / 180 - lat1 * M_PI / 180;
    auto dLon = long2 * M_PI / 180 - long1 * M_PI / 180;
    auto a = sin(dLat / 2) * sin(dLat / 2) +
             cos(lat1 * M_PI / 180) * cos(lat2 * M_PI / 180) *
             sin(dLon / 2) * sin(dLon / 2);
    auto c = 2 * atan2(sqrt(a), sqrt(1 - a));
    auto d = R * c;
    return d; // km
}

void Map::boundingBoxCreated(glm::vec2 start, glm::vec2 end) {
    double startX, startY = 0;
    viewport()->geoCoordinates(start.x, start.y, startX, startY, Marble::GeoDataCoordinates::Degree);
    double endX, endY = 0;
    viewport()->geoCoordinates(end.x, end.y, endX, endY, Marble::GeoDataCoordinates::Degree);
    //Calculate the size of selected area in square km
    auto width = getDistance(startY, startY, startX, endX);
    auto height = getDistance(startY, endY, startX, startX);
    auto area = width * height;
    if (area < 100.0) {
        dragRect = glm::vec4(endX, endY, startX, startY);
        update();
        return;
    }
    std::string text = "Selected area is over 100km2 (" + std::to_string(area) +
                       ") and might take a while to create.\nDo you want to continue?";
    auto reply = QMessageBox::question(this, "Are you sure?", QString::fromStdString(text),
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        dragRect = glm::vec4(endX, endY, startX, startY);
    } else {
        dragRect = glm::vec4(0, 0, 0, 0);
    }
    update();
}

glm::vec4 Map::getSelectedArea() {
    return dragRect;
}

bool Map::hasSelectedArea() {
    return dragRect.x != 0 && dragRect.y != 0 && dragRect.z != 0 && dragRect.w != 0;
}



