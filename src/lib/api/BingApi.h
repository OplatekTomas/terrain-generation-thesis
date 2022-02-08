//
// Created by tomas on 30.09.21.
//


#pragma once

#include <api/ApiBase.h>
#include <api/models/bing/ElevationResult.h>
#include <api/models/bing/ElevationData.h>

namespace MapGenerator {
    class BingApi : ApiBase {
    public:
        explicit BingApi(std::string key);

        std::shared_ptr<ElevationData>
        getElevation(double lat1, double long1, double lat2, double long2, int resolution);

        std::shared_ptr<ElevationData>
        getElevationNormalized(double lat1, double long1, double lat2, double long2, int resolution);

    private:
        std::string getBaseAddress() override;

        std::shared_ptr<ElevationResult>
        sendElevationRequest(double lat1, double long1, double lat2, double long2, int rows, int cols);


        static double getDistanceBetweenPoints(double lat1, double long1, double lat2, double long2);


    };
}

