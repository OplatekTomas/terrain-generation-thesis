//
// Created by tomas on 03.11.21.
//

#include <common/AreaOnMap.h>

namespace MapGenerator{

    AreaOnMap::AreaOnMap() {

    }

    bool AreaOnMap::isInside(double lat, double lon) {
        return false;
    }

    void AreaOnMap::getColor(char *r, char *g, char *b) {
        //TODO do this
    }

    void AreaOnMap::addNode(const element &el) {
        points.push_back(el);
        latMin = *el.lat < latMin ? *el.lat : latMin;
        latMax = *el.lat > latMax ? *el.lat : latMax;
        lonMin = *el.lon < lonMin ? *el.lon : lonMin;
        lonMax = *el.lon > lonMax ? *el.lon : lonMax;
    }

}

