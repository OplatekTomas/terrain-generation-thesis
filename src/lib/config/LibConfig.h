//
// Created by tomas on 04.10.21.
//

#pragma once


#include <nlohmann/json.hpp>
#include <JsonHelper.h>

namespace MapGenerator {
    using nlohmann::json;

    struct Key {
        std::string service;
        std::string key;
    };

    struct GeneratorOptions {
        double lat1{};
        double lat2{};
        double lon1{};
        double lon2{};
        int64_t terrainResolution;
        int64_t minTextureResolution;
        int64_t maxTextureResolution;
        int64_t textureResolutionStep;
        int64_t generateVegetationAt;
        int64_t randomSeed;
    };

    struct LibConfig {
        std::vector<Key> keys;
        GeneratorOptions options;
    };
}

namespace nlohmann {
    void from_json(const json & j, MapGenerator::Key & x);
    void to_json(json & j, const MapGenerator::Key & x);

    void from_json(const json & j, MapGenerator::GeneratorOptions & x);
    void to_json(json & j, const MapGenerator::GeneratorOptions & x);

    void from_json(const json & j, MapGenerator::LibConfig & x);
    void to_json(json & j, const MapGenerator::LibConfig & x);

    inline void from_json(const json & j, MapGenerator::Key& x) {
        x.service = j.at("service").get<std::string>();
        x.key = j.at("key").get<std::string>();
    }

    inline void to_json(json & j, const MapGenerator::Key & x) {
        j = json::object();
        j["service"] = x.service;
        j["key"] = x.key;
    }

    inline void from_json(const json & j, MapGenerator::GeneratorOptions& x) {
        x.terrainResolution = j.at("terrainResolution").get<int64_t>();
        x.minTextureResolution = j.at("minTextureResolution").get<int64_t>();
        x.maxTextureResolution = j.at("maxTextureResolution").get<int64_t>();
        x.textureResolutionStep = j.at("textureResolutionStep").get<int64_t>();
        x.generateVegetationAt = j.at("generateVegetationAt").get<int64_t>();
        x.randomSeed = j.at("randomSeed").get<int64_t>();
    }

    inline void to_json(json & j, const MapGenerator::GeneratorOptions & x) {
        j = json::object();
        j["terrainResolution"] = x.terrainResolution;
        j["minTextureResolution"] = x.minTextureResolution;
        j["maxTextureResolution"] = x.maxTextureResolution;
        j["textureResolutionStep"] = x.textureResolutionStep;
        j["generateVegetationAt"] = x.generateVegetationAt;
        j["randomSeed"] = x.randomSeed;
    }

    inline void from_json(const json & j, MapGenerator::LibConfig& x) {
        x.keys = j.at("keys").get<std::vector<MapGenerator::Key>>();
        x.options = j.at("options").get<MapGenerator::GeneratorOptions>();
    }

    inline void to_json(json & j, const MapGenerator::LibConfig & x) {
        j = json::object();
        j["keys"] = x.keys;
        j["options"] = x.options;
    }
}

