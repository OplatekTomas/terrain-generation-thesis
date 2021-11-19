//
// Created by tomas on 14.10.21.
//

#include <models/openstreetmap/MetadataResult.h>
#include "OpenStreetMapApi.h"
#include <iostream>
#include <fmt/format.h>

namespace MapGenerator {

    std::string OpenStreetMapApi::getBaseAddress() {
        return "http://overpass-api.de/api/interpreter";
    }

    std::shared_ptr<OSMData>
    OpenStreetMapApi::getMetadata(double lat1, double long1, double lat2, double long2) {

        auto query = "[out:json];\n(node({0},{1},{2},{3});<;);out meta;";
        auto queryResult = encodeUrl(fmt::format(query, lat1, long1, lat2, long2));
        auto url = getBaseAddress() + "?data=" + queryResult;

        //auto result = this->readData<MetadataResult>("../../../examples/osm.json");

        auto result = this->sendRequest<MetadataResult>(url);
        if (result == nullptr) {
            return nullptr;
        }
        auto data = std::make_shared<OSMData>(result);
        auto missingNodes = data->getMissingNodes();
        if (missingNodes.empty()) {
            return data;
        }

        auto missingQuery = "[out:json];node(id:{0});out meta;";
        auto missingQueryResult = fmt::format(missingQuery, fmt::join(missingNodes, ","));
        auto missingUrl = getBaseAddress();
        auto missing = this->postRequest<MetadataResult>(missingUrl, "application/x-www-form-urlencoded",
                                                         "data=" + missingQueryResult);
        if (missing == nullptr) {
            return nullptr;
        }
        for (const auto &node: missing->elements) {
            data->addNode(node);
        }
        return data;
    }


    MapGenerator::OpenStreetMapApi::OpenStreetMapApi(std::string key) : ApiBase(key) {

    }

};
