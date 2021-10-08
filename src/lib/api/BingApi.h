//
// Created by tomas on 30.09.21.
//


#pragma once

#include <api/ApiBase.h>
#include <models/bing/ElevationResult.h>

namespace MapGenerator {
    class BingApi : ApiBase {
    public:
        explicit BingApi(std::string key);
        std::shared_ptr<ElevationResult> getElevation(double long1, double lat1, double long2, double lat2, int resolution = 10);
        std::vector<double> getElevationNormalized(double long1, double lat1, double long2, double lat2, int resolution = 10);

    private:
        std::string getBaseAddress() override;

    };
}

