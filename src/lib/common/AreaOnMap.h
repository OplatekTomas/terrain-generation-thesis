//
// Created by tomas on 03.11.21.
//

#pragma once

#include <vector>
#include <models/openstreetmap/MetadataResult.h>
#include <bits/stdc++.h>


namespace MapGenerator{
    class AreaOnMap {
    public:
        AreaOnMap();
        bool isInsideArea(double lat, double lon);

        bool isInsideBounds(double lat, double lon);
        void getColor(char* r, char* g, char* b);
        void addNode(const element& el);
        double minLat = DBL_MAX;
        double maxLat = DBL_TRUE_MIN;
        double minLon = DBL_MAX;
        double maxLon = DBL_TRUE_MIN;

    private:
        std::vector<element> points;


        bool isInsidePolygon(double lat, double lon);
    };
}





