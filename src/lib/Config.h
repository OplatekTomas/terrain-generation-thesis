//
// Created by tomas on 04.10.21.
//

#pragma once


#include <nlohmann/json.hpp>
#include <JsonHelper.h>
namespace MapGenerator {
    using nlohmann::json;

    struct key {
        std::string service;
        std::string key;
    };

    typedef struct Config_T {
        std::vector<key> keys;
    } Config;
}

namespace nlohmann {
    void from_json(const json & j, MapGenerator::key & x);
    void to_json(json & j, const MapGenerator::key & x);

    void from_json(const json & j, MapGenerator::Config & x);
    void to_json(json & j, const MapGenerator::Config & x);

    inline void from_json(const json & j, MapGenerator::key& x) {
        x.service = j.at("service").get<std::string>();
        x.key = j.at("key").get<std::string>();
    }

    inline void to_json(json & j, const MapGenerator::key & x) {
        j = json::object();
        j["service"] = x.service;
        j["key"] = x.key;
    }

    inline void from_json(const json & j, MapGenerator::Config& x) {
        x.keys = j.at("keys").get<std::vector<MapGenerator::key>>();
    }

    inline void to_json(json & j, const MapGenerator::Config & x) {
        j = json::object();
        j["keys"] = x.keys;
    }
}

