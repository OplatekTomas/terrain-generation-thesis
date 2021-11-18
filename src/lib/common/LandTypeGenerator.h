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
#include <Helper.h>
#include <common/AreaOnMap.h>
#include <models/openstreetmap/OSMData.h>

namespace MapGenerator {
    using namespace std;
    using namespace boolinq;


    class LandTypeGenerator {
    public:
        LandTypeGenerator(double lat1, double lon1, double lat2, double lon2, int resolution, shared_ptr<OSMData> osmData);

        shared_ptr<vector<unsigned char>> generateTexture();

    private:
        void generateAreas(shared_ptr<OSMData> osmData);

        std::shared_ptr<OSMData> osmData;
        map<element, AreaOnMap> areas;
        int resolution;
        double xSize;
        double ySize;
        double xStep;
        double yStep;
        double xStart;
        double yStart;
    };


}



