//
// Created by tomas on 14.10.21.
//

#include <api/models/openstreetmap/MetadataResult.h>
#include "OpenStreetMapApi.h"
#include <iostream>
#include <tuple>
#include <fmt/format.h>
#include <Logger.h>

namespace MapGenerator {

    std::string OpenStreetMapApi::getBaseAddress() {
        return "http://lz4.overpass-api.de/api/interpreter";
    }

    std::shared_ptr<OSMData>
    OpenStreetMapApi::getMetadata(double lat1, double lon1, double lat2, double lon2) {
        //start time
        auto start = std::chrono::high_resolution_clock::now();
        Logger::log("Downloading metadata from OpenStreetMap API");
        auto query = "[out:json];\n(node({0},{1},{2},{3});<;);out geom;";
        auto queryResult = encodeUrl(fmt::format(query, lat1, lon1, lat2, lon2));
        auto url = getBaseAddress() + "?data=" + queryResult;

        //auto result = this->readData<MetadataResult>("../../../examples/osm.json");

        int size = 0;
        auto result = this->sendRequest<MetadataResult>(url, &size);
        if (result == nullptr) {
            return nullptr;
        }
        auto data = std::make_shared<OSMData>(result);
        data->lat1 = lat1;
        data->lon1 = lon1;
        data->lat2 = lat2;
        data->lon2 = lon2;
        //end time
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        Logger::log("Metadata downloaded in " + std::to_string(duration.count()) + "s");
        Logger::log("Metadata size " + std::to_string(size / (1024*1024)) + "MB");
        return data;
    }

    MapGenerator::OpenStreetMapApi::OpenStreetMapApi(std::string key) : ApiBase(key) {

    }

};
