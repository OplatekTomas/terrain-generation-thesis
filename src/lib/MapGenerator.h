//
// Created by tomas on 08.10.21.
//

#pragma once

#include <config/Config.h>
#include <api/BingApi.h>
#include <VertexData.h>

namespace MapGenerator{
    class MapGenerator {
    public:
        explicit MapGenerator(Config* config);
        std::shared_ptr<VertexData> getVertices(double lat1, double long1, double lat2, double long2, int resolution = 20);
    private:
        Config* config;
        std::unique_ptr<BingApi> bing;
    };
}




