//
// Created by tomas on 03.11.21.
//

#include <common/AreaOnMap.h>
#include <boolinq.h>

namespace MapGenerator {

    AreaOnMap::AreaOnMap() {

    }

    bool AreaOnMap::isInsideBounds(double lat, double lon) {
        return (lat >= minLat && lat <= maxLat && lon >= minLon && lon <= maxLon);
    }

    bool AreaOnMap::isInsideArea(double lat, double lon) {
        //Determine if the point is inside area defined by points

        //Check if the point is inside the bounds
        if (!isInsideBounds(lat, lon)) {
            return false;
        }
        //Check if point is inside the polygon
        if (isInsidePolygon(lat, lon)) {
            return true;
        }
        return false;
        //return c;
    }

    bool AreaOnMap::isInsidePolygon(double lat, double lon) {
        //Determine if the point is inside area defined by points
        //Check if point is inside the polygon
        int i, j;
        bool c = false;
        for (i = 0, j = points.size() - 1; i < points.size(); j = i++) {
            if (((*points[i].lat <= lat && lat < *points[j].lat) || (*points[j].lat <= lat && lat < *points[i].lat)) &&
                (lon < (*points[j].lon - *points[i].lon) * (lat - *points[i].lat) / (*points[j].lat - *points[i].lat) +
                       *points[i].lon)) {
                c = !c;
            }
        }
        return c;
    }

    bool AreaOnMap::isInsideLon(double lon) {
        return (lon >= minLon && lon <= maxLon);
    }


    void AreaOnMap::getColor(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) {
        *r = this->r;
        *g = this->g;
        *b = this->b;
        *a = 255;
    }

    void AreaOnMap::addNode(const element &el) {
        points.push_back(el);
        minLat = *el.lat < minLat ? *el.lat : minLat;
        maxLat = *el.lat > maxLat ? *el.lat : maxLat;
        minLon = *el.lon < minLon ? *el.lon : minLon;
        maxLon = *el.lon > maxLon ? *el.lon : maxLon;
        area = (maxLat - minLat) * (maxLon - minLon);

    }

}

