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
        std::shared_ptr<ElevationResult> getElevation(double lat1, double long1, double lat2, double long2, int rows = 10, int cols = 10);
        std::tuple<std::vector<double>, double, double>
        getElevationNormalized(double lat1, double long1, double lat2, double long2, int resolution = 10);

    private:
        std::string getBaseAddress() override;




        static double getDistanceBetweenPoints(double lat1, double long1, double lat2, double long2);
    };
}

