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

        bool isInside(double lat, double lon);
        void getColor(char* r, char* g, char* b);
        void addNode(const element& el);
        double latMin = DBL_MAX;
        double latMax = DBL_TRUE_MIN;
        double lonMin = DBL_MAX;
        double lonMax = DBL_TRUE_MIN;

    private:
        std::vector<element> points;
    };
}





