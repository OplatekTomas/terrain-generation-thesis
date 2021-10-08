//
// Created by tomas on 30.09.21.
//

#pragma once


#include <nlohmann/json.hpp>
#include <JsonHelper.h>

#pragma once

namespace MapGenerator {
    using nlohmann::json;

    struct Resource {
        std::string type;
        std::vector<double> elevations;
        int64_t zoomLevel;
    };

    struct ResourceSet {
        int64_t estimatedTotal;
        std::vector<Resource> resources;
    };

    struct ElevationResult   {
        std::string authenticationResultCode;
        std::string brandLogoUri;
        std::string copyright;
        std::vector<ResourceSet> resourceSets;
        int64_t statusCode;
        std::string statusDescription;
        std::string traceId;
    };
}

namespace nlohmann {
    void from_json(const json & j, MapGenerator::Resource & x);
    void to_json(json & j, const MapGenerator::Resource & x);

    void from_json(const json & j, MapGenerator::ResourceSet & x);
    void to_json(json & j, const MapGenerator::ResourceSet & x);

    void from_json(const json & j, MapGenerator::ElevationResult & x);
    void to_json(json & j, const MapGenerator::ElevationResult & x);

    inline void from_json(const json & j, MapGenerator::Resource& x) {
        x.type = j.at("__type").get<std::string>();
        x.elevations = j.at("elevations").get<std::vector<double>>();
        x.zoomLevel = j.at("zoomLevel").get<int64_t>();
    }

    inline void to_json(json & j, const MapGenerator::Resource & x) {
        j = json::object();
        j["__type"] = x.type;
        j["elevations"] = x.elevations;
        j["zoomLevel"] = x.zoomLevel;
    }

    inline void from_json(const json & j, MapGenerator::ResourceSet& x) {
        x.estimatedTotal = j.at("estimatedTotal").get<int64_t>();
        x.resources = j.at("resources").get<std::vector<MapGenerator::Resource>>();
    }

    inline void to_json(json & j, const MapGenerator::ResourceSet & x) {
        j = json::object();
        j["estimatedTotal"] = x.estimatedTotal;
        j["resources"] = x.resources;
    }

    inline void from_json(const json & j, MapGenerator::ElevationResult& x) {
        x.authenticationResultCode = j.at("authenticationResultCode").get<std::string>();
        x.brandLogoUri = j.at("brandLogoUri").get<std::string>();
        x.copyright = j.at("copyright").get<std::string>();
        x.resourceSets = j.at("resourceSets").get<std::vector<MapGenerator::ResourceSet>>();
        x.statusCode = j.at("statusCode").get<int64_t>();
        x.statusDescription = j.at("statusDescription").get<std::string>();
        x.traceId = j.at("traceId").get<std::string>();
    }

    inline void to_json(json & j, const MapGenerator::ElevationResult & x) {
        j = json::object();
        j["authenticationResultCode"] = x.authenticationResultCode;
        j["brandLogoUri"] = x.brandLogoUri;
        j["copyright"] = x.copyright;
        j["resourceSets"] = x.resourceSets;
        j["statusCode"] = x.statusCode;
        j["statusDescription"] = x.statusDescription;
        j["traceId"] = x.traceId;
    }
}

