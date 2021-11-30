//
// Created by tomas on 03.11.21.
//

#pragma once

#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <models/openstreetmap/MetadataResult.h>
#include <boolinq.h>
#include <common/AreaOnMap.h>
#include <models/openstreetmap/OSMData.h>

namespace MapGenerator {
    using namespace std;
    using namespace boolinq;


    class LandTypeGenerator {
    public:
        LandTypeGenerator(double lat1, double lon1, double lat2, double lon2, shared_ptr<OSMData> osmData);

        shared_ptr <vector<float>> generateTexture(int resolution);

    private:
        std::shared_ptr<OSMData> osmData;
        std::vector<std::shared_ptr<AreaOnMap>> areas;
        int resolution;
        double xSize;
        double ySize;
        double xStep;
        double yStep;
        double xStart;
        double yStart;

        void prepareAreas();

    };


}



