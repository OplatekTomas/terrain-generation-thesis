//
// Created by tomas on 03.11.21.
//

#include <common/AreaOnMap.h>
#include <boolinq.h>
#include <Helper.h>

namespace MapGenerator {

    AreaOnMap::AreaOnMap(const Node &elem) {


        this->node = elem;
        this->isRelation = elem.type == Type::relation;
        minLon = elem.bounds->minLon;
        maxLon = elem.bounds->maxLon;
        minLat = elem.bounds->minLat;
        maxLat = elem.bounds->maxLat;
        area = (maxLat - minLat) * (maxLon - minLon);

        resolveColor();
        computeRoute();
    }

    AreaOnMap::AreaOnMap() {
    }

    void AreaOnMap::computeRoute() {
        if(!mapContainsKey(*node.tags, "highway")) {
            return;
        }
        auto highwayType = node.tags->at("highway");
        switch(highwayType)
    }


    void AreaOnMap::resolveColor() {
        bool randColor = false;
        auto tags = *node.tags.get();
        //Check if area is covered by a forest

        if (randColor) {
            this->r = rand() % 255;
            this->g = rand() % 255;
            this->b = rand() % 255;
            return;
        }

        if (mapContainsKeyAndValue(tags, "natural", "wood") || mapContainsKeyAndValue(tags, "landuse", "forest")) {
            this->r = 0;
            this->g = 74;
            this->b = 11;
        }//Check for water
        else if (mapContainsKeyAndValue(tags, "natural", "water") ||
                 mapContainsKeyAndValue(tags, "waterway", "riverbank") ||
                 mapContainsKeyAndValue(tags, "waterway", "river") ||
                 mapContainsKeyAndValue(tags, "waterway", "stream")) {
            this->r = 0;
            this->g = 0;
            this->b = 255;
        }//Check for fields
        else if (mapContainsKeyAndValue(tags, "landuse", "farm") ||
                 mapContainsKeyAndValue(tags, "landuse", "farmyard") ||
                 mapContainsKeyAndValue(tags, "landuse", "farmland") ||
                 mapContainsKeyAndValue(tags, "landuse", "orchard") ||
                 mapContainsKeyAndValue(tags, "landuse", "vineyard") ||
                 mapContainsKeyAndValue(tags, "landuse", "meadow")) {
            this->r = 200;
            this->g = 200;
            this->b = 0;
        } // Check for roads
        else if (mapContainsKeyAndValue(tags, "highway", "motorway") ||
                 mapContainsKeyAndValue(tags, "highway", "motorway_link") ||
                 mapContainsKeyAndValue(tags, "highway", "trunk") ||
                 mapContainsKeyAndValue(tags, "highway", "trunk_link") ||
                 mapContainsKeyAndValue(tags, "highway", "primary") ||
                 mapContainsKeyAndValue(tags, "highway", "primary_link") ||
                 mapContainsKeyAndValue(tags, "highway", "secondary") ||
                 mapContainsKeyAndValue(tags, "highway", "secondary_link") ||
                 mapContainsKeyAndValue(tags, "highway", "tertiary") ||
                 mapContainsKeyAndValue(tags, "highway", "tertiary_link") ||
                 mapContainsKeyAndValue(tags, "highway", "unclassified") ||
                 mapContainsKeyAndValue(tags, "highway", "residential") ||
                 mapContainsKeyAndValue(tags, "highway", "service") ||
                 mapContainsKeyAndValue(tags, "highway", "track") ||
                 mapContainsKeyAndValue(tags, "highway", "road") ||
                 mapContainsKeyAndValue(tags, "highway", "living_street") ||
                 mapContainsKeyAndValue(tags, "highway", "pedestrian") ||
                 mapContainsKeyAndValue(tags, "highway", "footway") ||
                 mapContainsKeyAndValue(tags, "highway", "steps") ||
                 mapContainsKeyAndValue(tags, "highway", "path") ||
                 mapContainsKeyAndValue(tags, "highway", "cycleway") ||
                 mapContainsKeyAndValue(tags, "highway", "bridleway") ||
                 mapContainsKeyAndValue(tags, "highway", "construction") ||
                 mapContainsKeyAndValue(tags, "highway", "proposed")){
            this->r = 255;
            this->g = 0;
            this->b = 0;
        } // Check for buildings and make them grey
        else if (mapContainsKey(tags, "building")){
            this->r = 160;
            this->g = 160;
            this->b = 160;
        } // Check for other stuff
        else {
            this->r = 255;
            this->g = 255;
            this->b = 255;
        }
    }


    bool AreaOnMap::isInsideBounds(double lat, double lon) {
        return (lat >= minLat && lat <= maxLat && lon >= minLon && lon <= maxLon);
    }

    bool AreaOnMap::isInsideArea(double lat, double lon) {
        //Check if the point is inside the bounds
        if (!isInsideBounds(lat, lon)) {
            return false;
        }
        if (isRelation) {
            return isInsideRelation(lat, lon);
        } else {
            return isInsideWay(lat, lon, node);
        }
    }

    bool AreaOnMap::isInsideRelation(double lat, double lon) {
        return std::any_of(node.members->begin(), node.members->end(), [&](const Member &member) {
            if (member.type == Type::way) {
                return isInsideWay(lat, lon, member);
            }
            return false;
        });
    }

    bool AreaOnMap::isInsideWay(double lat, double lon, const ShapeBase &way) {
        //Determine if the point is inside area defined by points
        //Check if point is inside the polygon
        int i, j;
        bool c = false;
        auto points = *way.geometry;
        if(points.at(points.size() - 1).lat != points.at(0).lat || points.at(points.size() - 1).lon != points.at(0).lon) {
            return false;
        }
        for (i = 0, j = points.size() - 1; i < points.size(); j = i++) {
            if (((points[i].lat <= lat && lat < points[j].lat) || (points[j].lat <= lat && lat < points[i].lat)) &&
                (lon < (points[j].lon - points[i].lon) * (lat - points[i].lat) / (points[j].lat - points[i].lat) +
                       points[i].lon)) {
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
}

