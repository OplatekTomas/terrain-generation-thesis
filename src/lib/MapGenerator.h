//
// Created by tomas on 08.10.21.
//

#pragma once

#include <Config.h>
#include <api/BingApi.h>

namespace MapGenerator{
    class MapGenerator {
    public:
        explicit MapGenerator(Config* config);

        void getVertices();
    private:
        Config* config;
        std::unique_ptr<BingApi> bing;
    };
}




