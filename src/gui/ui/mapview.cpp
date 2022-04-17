//
// Created by tomas on 12.04.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MapView.h" resolved

#include <ui/mapview.h>
#include "ui_mapview.h"
#include "MainWindow.h"
#include <marble/MarbleModel.h>
#include <marble/SearchRunnerManager.h>
#include <marble/GeoDataPlacemark.h>
#include <QMessageBox>


MapView::MapView(QWidget *parent) : QWidget(parent), ui(new Ui::MapView) {
    ui->setupUi(this);
    vector<double> start = {49.19256141221154, 16.594543972568715,
            49.19827707820228, 16.604973078397315};
    auto geo = Marble::GeoDataLatLonBox(start[2], start[0], start[3], start[1], Marble::GeoDataCoordinates::Degree);
    ui->map->centerOn(geo, true);
    ui->map->zoomView(2200);
    ui->map->reloadMap();
    ui->map->update();
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
        ui->map->zoomView(2500, FlyToMode::Automatic);
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

void MapView::onRender() {
    if(!ui->map->hasSelectedArea()){
        QMessageBox::warning(this, "No area selected", "Please select an area to render");
        return;
    }
    //Get the selected area
    auto box = ui->map->getSelectedArea();
    //Get the current window to call the render function
    auto mainWindow = (MainWindow *) qApp->activeWindow();
    if(mainWindow == nullptr) {
        QMessageBox::warning(this, "No window", "No window is active - this should not happen");
        return;
    }
    mainWindow->drawRenderer(box);
}



