//
// Created by tomas on 14.10.21.
//

#include <models/openstreetmap/MetadataResult.h>
#include "OpenStreetMapApi.h"
#include <iostream>
#include <fmt/format.h>

namespace MapGenerator {

    std::string OpenStreetMapApi::getBaseAddress() {
        return "http://overpass-api.de/api/interpreter?data=";
    }

    std::shared_ptr<OSMData>
    OpenStreetMapApi::getMetadata(double lat1, double long1, double lat2, double long2) {

        auto query = "[out:json];\n(node({0},{1},{2},{3});<;);out meta;";
        auto queryResult = encodeUrl(fmt::format(query, lat1, long1, lat2, long2));
        auto url = getBaseAddress() + queryResult;

        auto result = this->readData<MetadataResult>("../../../examples/osm.json");

        //auto result = this->sendRequest<MetadataResult>(url);

        auto data = std::make_shared<OSMData>(result);
        auto missing = data->getMissingNodes();

        return data;
    }


    MapGenerator::OpenStreetMapApi::OpenStreetMapApi(std::string key) : ApiBase(key) {

    }

};
