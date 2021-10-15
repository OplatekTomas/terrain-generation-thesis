//
// Created by tomas on 30.09.21.
//

#include <api/BingApi.h>
#include <fmt/format.h>
#include <chrono>  // for high_resolution_clock


#include <utility>
#include <memory>
#include <csignal>
#include <cmath>

#define _USE_MATH_DEFINES


namespace MapGenerator {
    BingApi::BingApi(std::string key) : ApiBase(std::move(key)) {
    }

    std::string BingApi::getBaseAddress() {
        return "http://dev.virtualearth.net/REST/v1/";
    }

    std::shared_ptr<ElevationResult>
    BingApi::getElevation(double lat1, double long1, double lat2, double long2, int rows, int cols) {
        auto args = "Elevation/Bounds?bounds={0},{1},{2},{3}&rows={4}&cols={5}&key={6}";
        auto url = getBaseAddress() + fmt::format(args, lat1, long1, lat2, long2, rows, cols, this->apiKey);
        auto result = this->sendRequest<ElevationResult>(url);
        if (result == nullptr) {
            return nullptr;
        }
        return result;
    }



    std::tuple<std::vector<double>, double, double>
    BingApi::getElevationNormalized(double lat1, double long1, double lat2, double long2, int resolution) {
        auto result = getElevation(lat1, long1, lat2, long2, resolution, resolution);
        if (result == nullptr || result->resourceSets.empty() || result->resourceSets[0].resources.empty()) {
            return {};
        }
        auto data = result->resourceSets[0].resources[0].elevations;
        auto longDist = getDistanceBetweenPoints(lat1, long1, lat2, long1);
        auto latDist = getDistanceBetweenPoints(lat1, long1, lat1, long2);
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
        //this normalization is great but makes the world unrealistic - need to scale according to latitude and longitude as well
        for(int i = 0; i < data.size() - 3; i++){
            data[i] = (data[i] - min) / (max - min);
            minNormalized = data[i] < minNormalized ? data[i] : minNormalized;
            maxNormalized = data[i] > maxNormalized ? data[i] : maxNormalized;
        }
        data.resize(data.size()-3);
        auto tmp = (minNormalized - (1-maxNormalized)) / 2;
        for (auto& item: data) {
            item = item - tmp;
        }
        return {data, latDist, longDist};
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

}


