//
// Created by tomas on 14.10.21.
//

#pragma once

#include <api/ApiBase.h>
#include <models/openstreetmap/MetadataResult.h>
#include <models/openstreetmap/OSMData.h>

namespace MapGenerator{
    class OpenStreetMapApi : ApiBase {
    public:
        explicit OpenStreetMapApi(std::string key);
        std::string getBaseAddress() override;

        std::shared_ptr<OSMData> getMetadata(double lat1, double long1, double lat2, double long2);
    };
}




