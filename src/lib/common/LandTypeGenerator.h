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
#include "AreaOnMap.h"

namespace MapGenerator{
    using namespace std;
    using namespace boolinq;


    class LandTypeGenerator {
    public:
        LandTypeGenerator(double lat1, double lon1, double lat2, double lon2, int resolution);
        shared_ptr <vector<unsigned char>> generateTexture(const shared_ptr<MetadataResult>& metadata);
    private:
        const vector<string> tagTypes {"landuse", "highways", "waterways"};
        element getNode(long id);

        shared_ptr<MetadataResult> metadata;
        map<element, AreaOnMap> ways;
        vector<element> nodes;
        int resolution;
        double xSize;
        double ySize;
        double xStep;
        double yStep;
        double xStart;
        double yStart;

        element getNode(long id, int from, int to);
    };


}



