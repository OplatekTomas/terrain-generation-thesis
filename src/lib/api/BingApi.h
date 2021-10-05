//
// Created by tomas on 30.09.21.
//


#pragma once

#include "ApiBase.h"
#include "../models/bing/ElevationResult.h"

namespace MapGenerator {
    class BingApi : ApiBase {
    public:
        explicit BingApi(std::string key);
        ElevationResult * getElevation();
    private:
        std::string getBaseAddress() override;

    };
}

