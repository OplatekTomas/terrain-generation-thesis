//
// Created by tomas on 29.10.21.
//

#pragma once

#include <nlohmann/json.hpp>
#include <JsonHelper.h>
#include <unordered_map>

namespace MapGenerator {
    using nlohmann::json;

    enum class nodeType : int {
        node, relation, way
    };

    struct member {
        nodeType type;
        int64_t ref;
        std::string role;
    };


    struct element {
        nodeType type;
        int64_t id;
        std::shared_ptr<double> lat;
        std::shared_ptr<double> lon;
        std::string timestamp;
        int64_t version;
        int64_t changeset;
        std::string user;
        int64_t uid;
        std::shared_ptr<std::map<std::string, std::string>> tags;
        std::shared_ptr<std::vector<int64_t>> nodes;
        std::shared_ptr<std::vector<member>> members;

        bool operator <( const element &rhs ) const
        {
            return (id < rhs.id);
        }
    };

    struct osm3SInfo {
        std::string timestampOsmBase;
        std::string copyright;
    };

    struct MetadataResult {
        int64_t version;
        std::string generator;
        osm3SInfo osm3S;
        std::vector<element> elements;
    };
}

namespace nlohmann {
    void from_json(const json &j, MapGenerator::member &x);

    void to_json(json &j, const MapGenerator::member &x);

    void from_json(const json &j, MapGenerator::element &x);

    void to_json(json &j, const MapGenerator::element &x);

    void from_json(const json &j, MapGenerator::osm3SInfo &x);

    void to_json(json &j, const MapGenerator::osm3SInfo &x);

    void from_json(const json &j, MapGenerator::MetadataResult &x);

    void to_json(json &j, const MapGenerator::MetadataResult &x);

    void from_json(const json &j, MapGenerator::nodeType &x);

    void to_json(json &j, const MapGenerator::nodeType &x);

    inline void from_json(const json &j, MapGenerator::member &x) {
        x.type = j.at("type").get<MapGenerator::nodeType>();
        x.ref = j.at("ref").get<int64_t>();
        x.role = j.at("role").get<std::string>();
    }

    inline void to_json(json &j, const MapGenerator::member &x) {
        j = json::object();
        j["type"] = x.type;
        j["ref"] = x.ref;
        j["role"] = x.role;
    }

    inline void from_json(const json &j, MapGenerator::element &x) {
        x.type = j.at("type").get<MapGenerator::nodeType>();
        x.id = j.at("id").get<int64_t>();
        x.lat = MapGenerator::get_optional<double>(j, "lat");
        x.lon = MapGenerator::get_optional<double>(j, "lon");
        x.timestamp = j.at("timestamp").get<std::string>();
        x.version = j.at("version").get<int64_t>();
        x.changeset = j.at("changeset").get<int64_t>();
        x.user = j.at("user").get<std::string>();
        x.uid = j.at("uid").get<int64_t>();
        x.tags = MapGenerator::get_optional<std::map<std::string, std::string>>(j, "tags");
        x.nodes = MapGenerator::get_optional<std::vector<int64_t>>(j, "nodes");
        x.members = MapGenerator::get_optional<std::vector<MapGenerator::member>>(j, "members");
    }

    inline void to_json(json &j, const MapGenerator::element &x) {
        j = json::object();
        j["type"] = x.type;
        j["id"] = x.id;
        j["lat"] = x.lat == nullptr ? "null" : std::to_string(*x.lat);
        j["lon"] = x.lon == nullptr ? "null" : std::to_string(*x.lon);
        j["timestamp"] = x.timestamp;
        j["version"] = x.version;
        j["changeset"] = x.changeset;
        j["user"] = x.user;
        j["uid"] = x.uid;
        j["tags"] = *x.tags;
        j["nodes"] = *x.nodes;
        j["members"] = *x.members;
    }

    inline void from_json(const json &j, MapGenerator::osm3SInfo &x) {
        x.timestampOsmBase = j.at("timestamp_osm_base").get<std::string>();
        x.copyright = j.at("copyright").get<std::string>();
    }

    inline void to_json(json &j, const MapGenerator::osm3SInfo &x) {
        j = json::object();
        j["timestamp_osm_base"] = x.timestampOsmBase;
        j["copyright"] = x.copyright;
    }

    inline void from_json(const json &j, MapGenerator::MetadataResult &x) {
        x.version = j.at("version").get<int64_t>();
        x.generator = j.at("generator").get<std::string>();
        x.osm3S = j.at("osm3s").get<MapGenerator::osm3SInfo>();
        x.elements = j.at("elements").get<std::vector<MapGenerator::element>>();
    }

    inline void to_json(json &j, const MapGenerator::MetadataResult &x) {
        j = json::object();
        j["version"] = x.version;
        j["generator"] = x.generator;
        j["osm3s"] = x.osm3S;
        j["elements"] = x.elements;
    }

    inline void from_json(const json &j, MapGenerator::nodeType &x) {
        if (j == "node") x = MapGenerator::nodeType::node;
        else if (j == "relation") x = MapGenerator::nodeType::relation;
        else if (j == "way") x = MapGenerator::nodeType::way;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json &j, const MapGenerator::nodeType &x) {
        switch (x) {
            case MapGenerator::nodeType::node:
                j = "node";
                break;
            case MapGenerator::nodeType::relation:
                j = "relation";
                break;
            case MapGenerator::nodeType::way:
                j = "way";
                break;
            default:
                throw "This should not happen";
        }
    }


}

