//
// Created by tomas on 03.11.21.
//

#include <common/AreaOnMap.h>
#include <boolinq.h>
#include <Helper.h>

namespace MapGenerator {

    std::shared_ptr<AreaOnMap>
    AreaOnMap::create(double mapLatMin, double mapLatMax, double mapLonMin, double mapLonMax, Node &elem) {
        Bounds mapBounds{mapLatMin, mapLonMin, mapLatMax, mapLonMax};
        if (elem.type == Type::relation) {
            //Check if the relation is a boundary
            if (mapContainsKeyAndValue(*elem.tags, "boundary", "administrative")) {
                elem = createWayFromBoundary(elem);
            } else {
                auto members = elem.members;
                Bounds b{DBL_MAX, DBL_MAX, DBL_TRUE_MIN, DBL_TRUE_MIN};
                elem.members = std::make_shared<std::vector<Member>>();
                //Filter out ways outside of bounds - we don't need them
                for (const auto &way: *members) {
                    if (way.type != Type::way) {
                        continue;
                    }
                    auto bounds = calculateBounds(way);
                    //Check if bounds intersect mapBounds
                    if (!intersects(mapBounds, bounds)) {
                        continue;
                    }
                    elem.members->push_back(way);
                    for (const auto &point: *way.geometry) {
                        b.minLat = std::min(b.minLat, point.lat);
                        b.minLon = std::min(b.minLon, point.lon);
                        b.maxLat = std::max(b.maxLat, point.lat);
                        b.maxLon = std::max(b.maxLon, point.lon);
                    }
                }
                *elem.bounds = b;
            }
        }


        auto area = std::shared_ptr<AreaOnMap>(new AreaOnMap(elem));
        area->resolveColor();
        area->computeRoute();
        //this will force the area to calculate at this point (which is required)
        area->getArea();
        return area;
    }

    Node AreaOnMap::createWayFromBoundary(Node &elem) {
        Node way;
        way.type = Type::way;
        way.id = elem.id;
        way.tags = elem.tags;
        way.members = std::make_shared<std::vector<Member>>();
        way.geometry = std::make_shared<std::vector<Point>>();

        auto added = std::map<long, bool>();
        auto outerWays = boolinq::from(*elem.members)
                .where([](const Member &member) {
                    return member.type == Type::way && member.role == "outer";
                }).toStdVector();
        for (const auto &outerWay: outerWays) {
            added.insert({outerWay.ref, false});
        }

        auto currentWay = outerWays.at(0);
        bool shouldReverse = false;
        while (!added.find(currentWay.ref)->second) {
            added.find(currentWay.ref)->second = true;
            if (shouldReverse) {
                std::reverse(currentWay.geometry->begin(), currentWay.geometry->end());
            }
            for (const auto &node: *currentWay.geometry) {
                way.geometry->push_back(node);
            }
            auto lastNode = way.geometry->back();
            Member newCurrentWay;
            newCurrentWay = boolinq::from(outerWays).firstOrDefault([lastNode](const Member &member) {
                return member.geometry->front().lat == lastNode.lat && member.geometry->front().lon == lastNode.lon;
            });
            if (newCurrentWay.ref == 0) {
                shouldReverse = true;
                newCurrentWay = boolinq::from(outerWays).firstOrDefault([lastNode](const Member &member) {
                    return member.geometry->back().lat == lastNode.lat && member.geometry->back().lon == lastNode.lon;
                });
            }
            currentWay = newCurrentWay;
        }
        way.geometry->push_back(way.geometry->front());
        way.bounds = std::make_shared<Bounds>(calculateBounds(way));
        return way;
    }

    bool AreaOnMap::intersects(const Bounds &b1, const Bounds &b2) {
        return !(b1.maxLat < b2.minLat || b1.minLat > b2.maxLat || b1.maxLon < b2.minLon || b1.minLon > b2.maxLon);
    }


    Bounds AreaOnMap::calculateBounds(const ShapeBase &way) {
        Bounds b{DBL_MAX, DBL_MAX, DBL_TRUE_MIN, DBL_TRUE_MIN};
        for (auto &point: *way.geometry) {
            b.minLon = std::min(b.minLon, point.lon);
            b.minLat = std::min(b.minLat, point.lat);
            b.maxLon = std::max(b.maxLon, point.lon);
            b.maxLat = std::max(b.maxLat, point.lat);
        }
        return b;
    }

    AreaOnMap::AreaOnMap(const Node &elem) {

        this->isRoute = false;
        this->node = elem;
        this->isRelation = elem.type == Type::relation;
        min = {elem.bounds->minLat, elem.bounds->minLon};
        max = {elem.bounds->maxLat, elem.bounds->maxLon};

        //Calculate the area of a polygon
    }

    int AreaOnMap::getRouteWidth(const std::string &routeType) {
        int width = 0;
        switch (hash(routeType)) {
            case hash("motorway", 8):
            case hash("motorway_link", 13):
                width = 16;
                break;
            case hash("trunk", 5):
            case hash("trunk_link", 11):
                width = 12;
                break;
            case hash("primary", 7):
            case hash("primary_link", 12):
                width = 8;
                break;
            case hash("secondary", 9):
            case hash("secondary_link", 14):
                width = 6;
                break;
            case hash("tertiary", 8):
            case hash("tertiary_link", 13):
                width = 4;
                break;
            case hash("unclassified", 12):
                width = 2;
                break;
            case hash("residential", 11):
            case hash("living_street", 14):
                width = 4;
                break;
            case hash("footway", 7):
            case hash("path", 4):
            case hash("cycleway", 8):
            case hash("track", 5):
                width = 2;
                break;
        }
        return width;
    }

    void AreaOnMap::computeRoute() {
        if (!mapContainsKey(*node.tags, "highway")) {
            return;
        }
        auto highwayType = node.tags->at("highway");
        this->routeWidth = getRouteWidth(highwayType);
        if (routeWidth == 0) {
            return;
        }
        this->routeWidth = routeWidth / 111111.0;
        isRoute = true;
    }

    void AreaOnMap::resolveColor() {
        bool randColor = false;
        auto tags = *node.tags.get();
        //Check if area is covered by a forest

        if (randColor) {
            rgb[0] = rand() % 255;
            rgb[1] = rand() % 255;
            rgb[2] = rand() % 255;
        }

        std::vector<std::string> landUseTypes = {"grassland", "meadow", "orchard", "vineyard", "farmland"};

        std::vector<std::string> landUseLightTypes = {"village_green", "plant_nursery"};
        std::vector<std::string> leisureTypes = {"park", "garden", "plant_nursery", "pitch", "playground"};

        if (mapContainsKeyAndValue(tags, "natural", "wood") || mapContainsKeyAndValue(tags, "landuse", "forest")) {
            rgb[0] = 0;
            rgb[1] = 74;
            rgb[2] = 11;
        }//Check for water
        else if (mapContainsKeyAndValue(tags, "natural", "water") ||
                 mapContainsKeyAndValue(tags, "waterway", "riverbank") ||
                 mapContainsKeyAndValue(tags, "waterway", "river") ||
                 mapContainsKeyAndValue(tags, "waterway", "stream")) {
            rgb[0] = 0;
            rgb[1] = 0;
            rgb[2] = 255;

        }//Check for fields
        else if (boolinq::from(landUseTypes).any([&tags](const std::string &type) {
            return mapContainsKeyAndValue(tags, "landuse", type);
        })) {
            rgb[0] = 200;
            rgb[1] = 200;
            rgb[2] = 0;
        } // Check for roads
        else if (mapContainsKey(tags, "highway")) {
            rgb[0] = 179;
            rgb[1] = 81;
            rgb[2] = 20;
            priority = 10;
        } // Check for buildings
        else if (mapContainsKey(tags, "building")) {
            rgb[0] = 200;
            rgb[1] = 200;
            rgb[2] = 200;
            priority = 2;
        } // Check for leisure
        else if (boolinq::from(leisureTypes).any([&](const std::string &type) {
            return mapContainsKeyAndValue(tags, "leisure", type);
        }) || boolinq::from(landUseLightTypes).any(
                [&](const std::string &type) { return mapContainsKeyAndValue(tags, "landuse", type); })) {
            rgb[0] = 0;
            rgb[1] = 128;
            rgb[2] = 0;
        } // Check if the area is residential
        else if (mapContainsKeyAndValue(tags, "landuse", "residential") ||
                 mapContainsKeyAndValue(tags, "landuse", "industrial")) {
            rgb[0] = 128;
            rgb[1] = 128;
            rgb[2] = 128;
        } else {
            rgb[0] = 128;
            rgb[1] = 128;
            rgb[2] = 128;
            //If the area is unknown, just make the priority low, so it will be drawn last
            priority = -1;
        }
    }


    bool AreaOnMap::isInsideBounds(double lat, double lon) {
        return (lat >= min.lat && lat <= max.lat && lon >= min.lon && lon <= max.lon);
    }

    bool AreaOnMap::isInsideArea(double lat, double lon) {
        //Check if the point is inside the bounds

        if (!isInsideBounds(lat, lon)) {
            return false;
        }
        Point p = {lat, lon};
        if (isRelation) {
            return isInsideRelation(p);
        } else {
            return isInsideWay(p, node);
        }
    }

    bool AreaOnMap::isInsideRelation(Point p) {
        return std::any_of(node.members->begin(), node.members->end(), [&](const Member &member) {
            if (member.type == Type::way) {
                return isInsideWay(p, member);
            }
            return false;
        });
    }


    double AreaOnMap::distanceToLine(Point p, Point a, Point b) {
        double x = p.lon - a.lon;
        double y = p.lat - a.lat;
        double u = b.lon - a.lon;
        double v = b.lat - a.lat;
        double t = (x * u + y * v) / (u * u + v * v);
        if (t < 0) {
            return sqrt(x * x + y * y);
        }
        if (t > 1) {
            double xx = p.lon - b.lon;
            double yy = p.lat - b.lat;
            return sqrt(xx * xx + yy * yy);
        }
        double xx = a.lon + t * u - p.lon;
        double yy = a.lat + t * v - p.lat;
        return sqrt(xx * xx + yy * yy);
    }

    bool AreaOnMap::isInsideRoute(Point p, const ShapeBase &shape) {
        //Check if the distance between the point and the line is less than routeWidth using distanceToLine
        double distance = 0;
        bool result = false;
        for (int i = 0; i < shape.geometry->size() - 1; i++) {
            distance = distanceToLine(p, shape.geometry->at(i), shape.geometry->at(i + 1));
            if (distance < routeWidth) {
                result = true;
            }
        }
        return result;
    }


    bool AreaOnMap::isInsideWay(Point p, const ShapeBase &way) {
        if (isRoute) {
            return isInsideRoute(p, way);
        }

        //Determine if the point is inside area defined by points
        //Check if point is inside the polygon
        int i, j;
        bool c = false;
        auto points = *way.geometry;
        if (points.at(points.size() - 1).lat != points.at(0).lat ||
            points.at(points.size() - 1).lon != points.at(0).lon) {
            return false;
        }
        for (i = 0, j = points.size() - 1; i < points.size(); j = i++) {
            if (((points[i].lat <= p.lat && p.lat < points[j].lat) ||
                 (points[j].lat <= p.lat && p.lat < points[i].lat)) &&
                (p.lon < (points[j].lon - points[i].lon) * (p.lat - points[i].lat) / (points[j].lat - points[i].lat) +
                         points[i].lon)) {
                c = !c;
            }
        }
        return c;
    }

    bool AreaOnMap::isInsideLon(double lon) {
        return (lon >= min.lon && lon <= max.lon);
    }


    void AreaOnMap::getColor(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) {
        *r = rgb[0];
        *g = rgb[1];
        *b = rgb[2];
        *a = 255;
    }

    double AreaOnMap::getArea() {
        if (isRoute) {
            return area;
        }
        if (area == -1) {
            if (node.type == Type::way) {
                for (int i = 0; i < node.geometry->size() - 1; i++) {
                    area += fabs(node.geometry->at(i).lon * node.geometry->at(i + 1).lat -
                                 node.geometry->at(i + 1).lon * node.geometry->at(i).lat);
                }
                area += fabs(node.geometry->back().lon * node.geometry->front().lat -
                             node.geometry->front().lon * node.geometry->back().lat);
                area /= 2;
            } else {
                for (const auto &member: *node.members) {
                    if (member.type != Type::way) {
                        continue;
                    }
                    for (int i = 0; i < member.geometry->size() - 1; i++) {
                        area += fabs(member.geometry->at(i).lon * member.geometry->at(i + 1).lat -
                                     member.geometry->at(i + 1).lon * member.geometry->at(i).lat);
                    }
                    area += fabs(member.geometry->back().lon * member.geometry->front().lat -
                                 member.geometry->front().lon * member.geometry->back().lat);
                    area /= 2;
                }
            }
            //area = fabs(area);
        }
        return area;
    }

}

