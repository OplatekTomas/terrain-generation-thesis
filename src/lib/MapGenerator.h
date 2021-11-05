//
// Created by tomas on 08.10.21.
//

#pragma once

#include <config/Config.h>
#include <api/BingApi.h>
#include <VertexData.h>
#include <api/OpenStreetMapApi.h>
#include <models/openstreetmap/MetadataResult.h>

namespace MapGenerator{
    class MapGenerator {
    public:
        explicit MapGenerator(Config* config);
        std::shared_ptr<VertexData> getVertices(double lat1, double long1, double lat2, double long2, int resolution = 20);
        std::shared_ptr<MetadataResult> getMetadata(double lat1, double long1, double lat2, double long2, int resolution);

    private:
        Config* config;
        std::unique_ptr<BingApi> bing;
        std::unique_ptr<OpenStreetMapApi> osm;

    };
}




