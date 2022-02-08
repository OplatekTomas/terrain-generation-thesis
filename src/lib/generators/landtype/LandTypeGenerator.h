//
// Created by tomas on 03.11.21.
//

#pragma once

#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <api/models/openstreetmap/MetadataResult.h>
#include <boolinq.h>
#include <generators/landtype/AreaOnMap.h>
#include <api/models/openstreetmap/OSMData.h>
#include <scene/Scene.h>

namespace MapGenerator {
    using namespace std;
    using namespace boolinq;


    class LandTypeGenerator {
    public:
        LandTypeGenerator(shared_ptr<OSMData> osmData);
        shared_ptr <Texture> generateTexture(int resolution);

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



