//
// Created by tomas on 30.09.21.
//

#include <api/BingApi.h>
#include <fmt/format.h>
#include <chrono>  // for high_resolution_clock


#include <utility>
#include <memory>
#include <csignal>

namespace MapGenerator {
    BingApi::BingApi(std::string key) : ApiBase(std::move(key)) {
    }

    std::string BingApi::getBaseAddress() {
        return "http://dev.virtualearth.net/REST/v1/";
    }

    std::shared_ptr<ElevationResult> BingApi::getElevation(double long1, double lat1, double long2, double lat2, int resolution) {
        auto args = "Elevation/Bounds?bounds={0},{1},{2},{3}&rows={4}&cols={4}&key={5}";
        auto url = getBaseAddress() + fmt::format(args, long1, lat1, long2, lat2, resolution, this->apiKey);
        auto result = this->sendRequest<ElevationResult>(url);
        if(result == nullptr){
            return nullptr;
        }
        return result;
    }

    std::vector<double> BingApi::getElevationNormalized(double long1, double lat1, double long2, double lat2, int resolution) {
        auto result = getElevation(long1, lat1, long2, lat2, resolution);
        if(result == nullptr || result->resourceSets.empty() || result->resourceSets[0].resources.empty()){
            return {};
        }
        auto data = result->resourceSets[0].resources[0].elevations;
        double min = 8849; //Mt. Everest
        double max = -11034; //Mariana Trench
        //I could use stl functions, but this way there is only 1 pass
        for(auto item : data){
            min = item < min ? item : min;
            max = item > max ? item : max;
        }
        //this normalization is great but makes the world unrealistic - need to scale according to latitude and longitude as well
        for(double & item : data){
            item = (item-min)/(max-min);
        }
        return data;
    }

}


