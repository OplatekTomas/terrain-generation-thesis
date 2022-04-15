//
// Created by tomas on 14.10.21.
//

#include <api/models/openstreetmap/MetadataResult.h>
#include "OpenStreetMapApi.h"
#include <iostream>
#include <tuple>
#include <fmt/format.h>

namespace MapGenerator {

    std::string OpenStreetMapApi::getBaseAddress() {
        return "http://lz4.overpass-api.de/api/interpreter";
    }

    std::shared_ptr<OSMData>
    OpenStreetMapApi::getMetadata(double lat1, double lon1, double lat2, double lon2) {

        auto query = "[out:json];\n(node({0},{1},{2},{3});<;);out geom;";
        auto queryResult = encodeUrl(fmt::format(query, lat1, lon1, lat2, lon2));
        auto url = getBaseAddress() + "?data=" + queryResult;

        //auto result = this->readData<MetadataResult>("../../../examples/osm.json");

        auto result = this->sendRequest<MetadataResult>(url);
        if (result == nullptr) {
            return nullptr;
        }
        auto data = std::make_shared<OSMData>(result);
        data->lat1 = lat1;
        data->lon1 = lon1;
        data->lat2 = lat2;
        data->lon2 = lon2;
        return data;
    }

    MapGenerator::OpenStreetMapApi::OpenStreetMapApi(std::string key) : ApiBase(key) {

    }

};
