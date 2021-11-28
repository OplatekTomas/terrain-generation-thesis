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
        static std::shared_ptr<AreaOnMap>
        create(double mapLatMin, double mapLatMax, double mapLonMin, double mapLonMax, Node& elem);

        bool isInsideArea(double lat, double lon);

        bool isInsideBounds(double lat, double lon);

        void getColor(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);

        bool isInsideLon(double lon);

        double getArea();
        double getPriority(){
            return priority;
        };

    private:

        Node node;
        bool isRelation;
        bool isRoute;
        double routeWidth;
        unsigned char rgb[3];
        double area = -1;
        int priority = 0;
        Point min;
        Point max;

        AreaOnMap(const Node &elem);
        AreaOnMap();
        void resolveColor();
        void computeRoute();

        bool isInsideWay(Point p, const ShapeBase &way);

        bool isInsideRelation(Point p);

        bool isInsideRoute(Point p, const ShapeBase &shape);

        static int getRouteWidth(const std::string &routeType);

        double distanceToLine(Point p, Point a, Point b);

        static Bounds calculateBounds(const ShapeBase &way);

        static bool intersects(const Bounds &b1, const Bounds &b2);

        static Node createWayFromBoundary(Node &elem);
    };
}





