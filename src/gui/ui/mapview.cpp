//
// Created by tomas on 12.04.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MapView.h" resolved

#include <ui/mapview.h>
#include "ui_mapview.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <api/OpenStreetMapApi.h>
#include <marble/MarbleWidget.h>
#include <marble/MarbleModel.h>
#include <marble/SearchRunnerManager.h>
#include <marble/GeoDataPlacemark.h>


MapView::MapView(QWidget *parent) : QWidget(parent), ui(new Ui::MapView) {
    ui->setupUi(this);
}

MapView::~MapView() {
    delete ui;
}

void MapView::onSearch() {
    //Get text from search box
    QString searchText = ui->searchBar->text();
    //Use the text to search using marbles geocoding api
    auto *model = new MarbleModel;

    auto *manager = new SearchRunnerManager(model);
    QVector<GeoDataPlacemark *> searchResult = manager->searchPlacemarks(searchText);
    if(!searchResult.empty()) {
        //If there is a result, center the map on the first result
        GeoDataCoordinates coordinates = searchResult.at(0)->coordinate();
        //Get zoom level for coordinate
        ui->map->centerOn(coordinates, true);
        ui->map->zoomView(2750, FlyToMode::Linear);
    }
    //Clear search box
    ui->searchBar->clear();
    //Delete the model
    delete model;
    delete manager;
    //Delete the search result
    for(auto *placemark : searchResult) {
        delete placemark;
    }
}

void MapView::onDefaultClicked() {
//Start the rendering here.
}



