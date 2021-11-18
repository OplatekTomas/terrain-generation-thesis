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
        void getColor(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);
        void addNode(const element& el);
        double getArea(){return area;};
    private:
        std::vector<element> points;

        unsigned char r = rand()%256;
        unsigned char g = rand()%256;
        unsigned char b = rand()%256;


        double area = 0;
        double minLat = DBL_MAX;
        double maxLat = DBL_TRUE_MIN;
        double minLon = DBL_MAX;
        double maxLon = DBL_TRUE_MIN;
        bool isInsidePolygon(double lat, double lon);

    };
}





