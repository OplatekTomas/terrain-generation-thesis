//
// Created by tomas on 03.11.21.
//

#pragma once

#include <vector>
#include <models/openstreetmap/MetadataResult.h>
#include <bits/stdc++.h>


namespace MapGenerator {
    class AreaOnMap {
    public:
        explicit AreaOnMap(const Node &way);

        AreaOnMap();

        bool isInsideArea(double lat, double lon);

        bool isInsideBounds(double lat, double lon);

        void getColor(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);

        bool isInsideLon(double lon);

        double getArea() { return area; };
    private:
        Node node;
        bool isRelation;

        unsigned char r;
        unsigned char g;
        unsigned char b;

        double area = 0;
        double minLat = DBL_MAX;
        double maxLat = DBL_TRUE_MIN;
        double minLon = DBL_MAX;
        double maxLon = DBL_TRUE_MIN;


        void resolveColor();

        bool isInsideWay(double lat, double lon, const ShapeBase &way);

        bool isInsideRelation(double lat, double lon);
    };
}





