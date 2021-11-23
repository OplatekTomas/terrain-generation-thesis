//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Welcome data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>
#include <JsonHelper.h>

namespace MapGenerator {
    using nlohmann::json;


    struct Bounds {
        double minLat;
        double minLon;
        double maxLat;
        double maxLon;
    };

    struct Point {
        double lat;
        double lon;
    };

    enum class Type : int {
        node, relation, way
    };

    struct ShapeBase{
        Type type;
        std::shared_ptr<std::vector<Point>> geometry;
    };

    struct Member : ShapeBase {
        int64_t ref;
        std::string role;
        std::shared_ptr<double> lat;
        std::shared_ptr<double> lon;
    };

    struct Node : ShapeBase {
        int64_t id;
        std::shared_ptr<double> lat;
        std::shared_ptr<double> lon;
        std::shared_ptr<std::map<std::string, std::string>> tags;
        std::shared_ptr<Bounds> bounds;
        std::shared_ptr<std::vector<int64_t>> nodes;
        std::shared_ptr<std::vector<Member>> members;
    };

    struct Osm3S {
        std::string timestampOsmBase;
        std::string copyright;
    };

    struct MetadataResult {
        double version;
        std::string generator;
        Osm3S osm3S;
        std::vector<Node> elements;
    };
}

namespace nlohmann {
    void from_json(const json &j, MapGenerator::Bounds &x);

    void to_json(json &j, const MapGenerator::Bounds &x);

    void from_json(const json &j, MapGenerator::Point &x);

    void to_json(json &j, const MapGenerator::Point &x);

    void from_json(const json &j, MapGenerator::Member &x);

    void to_json(json &j, const MapGenerator::Member &x);

    void from_json(const json &j, MapGenerator::Node &x);

    void to_json(json &j, const MapGenerator::Node &x);

    void from_json(const json &j, MapGenerator::Osm3S &x);

    void to_json(json &j, const MapGenerator::Osm3S &x);

    void from_json(const json &j, MapGenerator::MetadataResult &x);

    void to_json(json &j, const MapGenerator::MetadataResult &x);

    void from_json(const json &j, MapGenerator::Type &x);

    void to_json(json &j, const MapGenerator::Type &x);

    inline void from_json(const json &j, MapGenerator::Bounds &x) {
        x.minLat = j.at("minlat").get<double>();
        x.minLon = j.at("minlon").get<double>();
        x.maxLat = j.at("maxlat").get<double>();
        x.maxLon = j.at("maxlon").get<double>();
    }

    inline void to_json(json &j, const MapGenerator::Bounds &x) {
        j = json::object();
        j["minlat"] = x.minLat;
        j["minlon"] = x.minLon;
        j["maxlat"] = x.maxLat;
        j["maxlon"] = x.maxLon;
    }

    inline void from_json(const json &j, MapGenerator::Point &x) {
        x.lat = j.at("lat").get<double>();
        x.lon = j.at("lon").get<double>();
    }

    inline void to_json(json &j, const MapGenerator::Point &x) {
        j = json::object();
        j["lat"] = x.lat;
        j["lon"] = x.lon;
    }

    inline void from_json(const json &j, MapGenerator::Member &x) {
        x.type = j.at("type").get<MapGenerator::Type>();
        x.ref = j.at("ref").get<int64_t>();
        x.role = j.at("role").get<std::string>();
        x.geometry = MapGenerator::get_optional<std::vector<MapGenerator::Point>>(j, "geometry");
        x.lat = MapGenerator::get_optional<double>(j, "lat");
        x.lon = MapGenerator::get_optional<double>(j, "lon");
    }

    inline void to_json(json &j, const MapGenerator::Member &x) {
        j = json::object();
        j["type"] = x.type;
        j["ref"] = x.ref;
        j["role"] = x.role;
        if (x.geometry) {
            j["geometry"] = *x.geometry;
        }
        if (x.lat) {
            j["lat"] = *x.lat;
        }
        if (x.lon) {
            j["lon"] = *x.lon;
        }
    }

    inline void from_json(const json &j, MapGenerator::Node &x) {
        x.type = j.at("type").get<MapGenerator::Type>();
        x.id = j.at("id").get<int64_t>();
        x.lat = MapGenerator::get_optional<double>(j, "lat");
        x.lon = MapGenerator::get_optional<double>(j, "lon");
        x.tags = MapGenerator::get_optional<std::map<std::string, std::string>>(j, "tags");
        x.bounds = MapGenerator::get_optional<MapGenerator::Bounds>(j, "bounds");
        x.nodes = MapGenerator::get_optional<std::vector<int64_t>>(j, "nodes");
        x.geometry = MapGenerator::get_optional<std::vector<MapGenerator::Point>>(j, "geometry");
        x.members = MapGenerator::get_optional<std::vector<MapGenerator::Member>>(j, "members");
    }

    inline void to_json(json &j, const MapGenerator::Node &x) {
        j = json::object();
        j["type"] = x.type;
        j["id"] = x.id;
        if (x.lat) {
            j["lat"] = *x.lat;
        }
        if (x.lon) {
            j["lon"] = *x.lon;
        }
        if (x.tags) {
            j["tags"] = *x.tags;
        }
        if (x.bounds) {
            j["bounds"] = *x.bounds;
        }
        if (x.nodes) {
            j["nodes"] = *x.nodes;
        }
        if (x.geometry) {
            j["geometry"] = *x.geometry;
        }
        if (x.members) {
            j["members"] = *x.members;
        }
    }

    inline void from_json(const json &j, MapGenerator::Osm3S &x) {
        x.timestampOsmBase = j.at("timestamp_osm_base").get<std::string>();
        x.copyright = j.at("copyright").get<std::string>();
    }

    inline void to_json(json &j, const MapGenerator::Osm3S &x) {
        j = json::object();
        j["timestamp_osm_base"] = x.timestampOsmBase;
        j["copyright"] = x.copyright;
    }

    inline void from_json(const json &j, MapGenerator::MetadataResult &x) {
        x.version = j.at("version").get<double>();
        x.generator = j.at("generator").get<std::string>();
        x.osm3S = j.at("osm3s").get<MapGenerator::Osm3S>();
        x.elements = j.at("elements").get<std::vector<MapGenerator::Node>>();
    }

    inline void to_json(json &j, const MapGenerator::MetadataResult &x) {
        j = json::object();
        j["version"] = x.version;
        j["generator"] = x.generator;
        j["osm3s"] = x.osm3S;
        j["elements"] = x.elements;
    }

    inline void from_json(const json &j, MapGenerator::Type &x) {
        if (j == "node") x = MapGenerator::Type::node;
        else if (j == "relation") x = MapGenerator::Type::relation;
        else if (j == "way") x = MapGenerator::Type::way;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json &j, const MapGenerator::Type &x) {
        switch (x) {
            case MapGenerator::Type::node:
                j = "node";
                break;
            case MapGenerator::Type::relation:
                j = "relation";
                break;
            case MapGenerator::Type::way:
                j = "way";
                break;
            default:
                throw "This should not happen";
        }
    }
}
