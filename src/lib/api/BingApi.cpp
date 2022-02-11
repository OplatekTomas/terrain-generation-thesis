//
// Created by tomas on 30.09.21.
//

#include <api/BingApi.h>
#include <fmt/format.h>
#include <chrono>  // for high_resolution_clock

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include <utility>
#include <memory>
#include <csignal>
#include <cmath>
#include <iostream>
#include <api/models/bing/ElevationData.h>

#define _USE_MATH_DEFINES


namespace MapGenerator {
    BingApi::BingApi(std::string key) : ApiBase(std::move(key)) {
    }

    std::string BingApi::getBaseAddress() {
        return "http://52.156.193.145/REST/v1/";
        return "http://dev.virtualearth.net/REST/v1/";
    }


    std::shared_ptr<ElevationData>
    BingApi::getElevation(double lat1, double long1, double lat2, double long2, int resolution) {
        //The points must be in a certain order for the api to work
        if (lat1 > lat2) {
            std::swap(lat1, lat2);
        }
        if (long1 < long2) {
            std::swap(long1, long2);
        }

        //If the resolution is lower than 32x32 we can just send 1 simple request
        if (resolution * resolution <= 1024) {
            auto result = sendElevationRequest(lat1, long1, lat2, long2, resolution, resolution);
            if (result == nullptr || result->resourceSets.empty() || result->resourceSets[0].resources.empty()) {
                return {};
            }
            return std::make_shared<ElevationData>(result->resourceSets[0].resources[0].elevations, resolution);
        }
        //If the resolution is higher than 32x32 we need to send multiple requests and compile the data together
        //Check the resolution and calculate the number of requests needed
        auto latStepSize = (lat2 - lat1) / resolution;
        auto longStepSize = (long1 - long2) / resolution;
        auto data = std::make_shared<ElevationData>(resolution, resolution);
        //Let's go through the requests and request 32 by 32 grid on each one
        for (int rowStart = 0; rowStart < resolution; rowStart += 32) {
            auto rowEnd = rowStart + 32;
            for (int colStart = 0; colStart < resolution; colStart += 32) {
                auto colEnd = colStart + 32;
                auto result = sendElevationRequest(
                        lat1 + (rowStart * latStepSize),
                        long2 + (colStart * longStepSize),
                        lat1 + (rowEnd * latStepSize),
                        long2 + (colEnd * longStepSize),
                        rowEnd - rowStart,
                        colEnd - colStart
                );
                auto elevations = result->resourceSets[0].resources[0].elevations;
                data->setAt(rowStart, rowEnd, colStart, colEnd, elevations);
            }
        }
        return data;
    }


    std::shared_ptr<ElevationData> BingApi::getElevationNormalized(double lat1, double long1, double lat2, double long2, int resolution) {
        auto elevationData = getElevation(lat1, long1, lat2, long2, resolution);
        //Get the distance of longitude and latitude in meters
        auto longDist = getDistanceBetweenPoints(lat1, long1, lat2, long1);
        auto latDist = getDistanceBetweenPoints(lat1, long1, lat1, long2);
        auto data = *elevationData->getData();
        //add latitude and longitude data for precise scaling
        data.push_back(0);
        data.push_back(longDist);
        data.push_back(latDist);

        double min = 8849; //Mt. Everest
        double max = -11034; //Mariana Trench
        double minNormalized = 8849;
        double maxNormalized = -11034;
        //I could use stl functions, but this way there is only 1 pass
        for (auto item: data) {
            min = item < min ? item : min;
            max = item > max ? item : max;
        }
        //normalize and get min/max of the normalized data without the last 3 (manually added) elements
        for (size_t i = 0; i < data.size() - 3; i++) {
            data[i] = (data[i] - min) / (max - min);
            minNormalized = data[i] < minNormalized ? data[i] : minNormalized;
            maxNormalized = data[i] > maxNormalized ? data[i] : maxNormalized;
        }
        data.resize(data.size() - 3);
        //Shift so that midpoint in the model is at 0.5
        auto shift = (minNormalized - (1 - maxNormalized)) / 2;
        for (auto &item: data) {
            item = item - shift;
        }
        auto result = std::make_shared<ElevationData>(data, resolution);
        result->setScale(longDist/latDist);
        result->setNormalizedMax(max);
        result->setNormalizedMin(min);
        return result;
    }

    double BingApi::getDistanceBetweenPoints(double lat1, double long1, double lat2, double long2) {
        const auto R = 6378.137; // Radius of earth in KM
        auto dLat = lat2 * M_PI / 180 - lat1 * M_PI / 180;
        auto dLon = long2 * M_PI / 180 - long1 * M_PI / 180;
        auto a = sin(dLat / 2) * sin(dLat / 2) +
                 cos(lat1 * M_PI / 180) * cos(lat2 * M_PI / 180) *
                 sin(dLon / 2) * sin(dLon / 2);
        auto c = 2 * atan2(sqrt(a), sqrt(1 - a));
        auto d = R * c;
        return d * 1000; // meters
    }

    std::shared_ptr<ElevationResult>
    BingApi::sendElevationRequest(double lat1, double long1, double lat2, double long2, int rows, int cols) {
        if (lat1 > lat2) {
            std::swap(lat1, lat2);
        }
        if (long1 < long2) {
            std::swap(long1, long2);
        }
        auto args = "Elevation/Bounds?bounds={0},{1},{2},{3}&rows={4}&cols={5}&key={6}";
        auto url = getBaseAddress() + fmt::format(args, lat1, long1, lat2, long2, rows, cols, this->apiKey);

        //TODO: return proper data
        auto result = this->readData<ElevationResult>("../../../examples/bing.json");

        //auto result = this->sendRequest<ElevationResult>(url);
        return result;
    }

}


