//
// Created by tomas on 14.10.21.
//

#include "OpenStreetMapApi.h"

std::string MapGenerator::OpenStreetMapApi::getBaseAddress() {
    return "https://api.openstreetmap.org/";
}

MapGenerator::OpenStreetMapApi::OpenStreetMapApi(std::string key) : ApiBase(key) {
    
}
