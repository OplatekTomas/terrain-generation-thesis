//
// Created by tomas on 30.09.21.
//

#ifndef VUT_BP_ELEVATIONRESULT_H
#define VUT_BP_ELEVATIONRESULT_H

#pragma once

#include "nlohmann/json.hpp"
#include "JsonHelper.h"

#include <optional>
#include <stdexcept>
#include <regex>

namespace MapGenerator {
    using nlohmann::json;

    class Resource {
    public:
        Resource() = default;
        virtual ~Resource() = default;

    private:
        std::string type;
        std::vector<int64_t> elevations;
        int64_t zoomLevel;

    public:
        const std::string & getType() const { return type; }
        std::string & getMutableType() { return type; }
        void setType(const std::string & value) { this->type = value; }

        const std::vector<int64_t> & getElevations() const { return elevations; }
        std::vector<int64_t> & getMutableElevations() { return elevations; }
        void setElevations(const std::vector<int64_t> & value) { this->elevations = value; }

        const int64_t & getZoomLevel() const { return zoomLevel; }
        int64_t & getMutableZoomLevel() { return zoomLevel; }
        void setZoomLevel(const int64_t & value) { this->zoomLevel = value; }
    };

    class ResourceSet {
    public:
        ResourceSet() = default;
        virtual ~ResourceSet() = default;

    private:
        int64_t estimatedTotal;
        std::vector<Resource> resources;

    public:
        const int64_t & getEstimatedTotal() const { return estimatedTotal; }
        int64_t & getMutableEstimatedTotal() { return estimatedTotal; }
        void setEstimatedTotal(const int64_t & value) { this->estimatedTotal = value; }

        const std::vector<Resource> & getResources() const { return resources; }
        std::vector<Resource> & getMutableResources() { return resources; }
        void setResources(const std::vector<Resource> & value) { this->resources = value; }
    };

    class ElevationResult {
    public:
        ElevationResult() = default;
        virtual ~ElevationResult() = default;

    private:
        std::string authenticationResultCode;
        std::string brandLogoUri;
        std::string copyright;
        std::vector<ResourceSet> resourceSets;
        int64_t statusCode;
        std::string statusDescription;
        std::string traceId;

    public:
        const std::string & getAuthenticationResultCode() const { return authenticationResultCode; }
        std::string & getMutableAuthenticationResultCode() { return authenticationResultCode; }
        void setAuthenticationResultCode(const std::string & value) { this->authenticationResultCode = value; }

        const std::string & getBrandLogoUri() const { return brandLogoUri; }
        std::string & getMutableBrandLogoUri() { return brandLogoUri; }
        void setBrandLogoUri(const std::string & value) { this->brandLogoUri = value; }

        const std::string & getCopyright() const { return copyright; }
        std::string & getMutableCopyright() { return copyright; }
        void setCopyright(const std::string & value) { this->copyright = value; }

        const std::vector<ResourceSet> & getResourceSets() const { return resourceSets; }
        std::vector<ResourceSet> & getMutableResourceSets() { return resourceSets; }
        void setResourceSets(const std::vector<ResourceSet> & value) { this->resourceSets = value; }

        const int64_t & getStatusCode() const { return statusCode; }
        int64_t & getMutableStatusCode() { return statusCode; }
        void setStatusCode(const int64_t & value) { this->statusCode = value; }

        const std::string & getStatusDescription() const { return statusDescription; }
        std::string & getMutableStatusDescription() { return statusDescription; }
        void setStatusDescription(const std::string & value) { this->statusDescription = value; }

        const std::string & getTraceId() const { return traceId; }
        std::string & getMutableTraceId() { return traceId; }
        void setTraceId(const std::string & value) { this->traceId = value; }
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
        x.setType(j.at("__type").get<std::string>());
        x.setElevations(j.at("elevations").get<std::vector<int64_t>>());
        x.setZoomLevel(j.at("zoomLevel").get<int64_t>());
    }

    inline void to_json(json & j, const MapGenerator::Resource & x) {
        j = json::object();
        j["__type"] = x.getType();
        j["elevations"] = x.getElevations();
        j["zoomLevel"] = x.getZoomLevel();
    }

    inline void from_json(const json & j, MapGenerator::ResourceSet& x) {
        x.setEstimatedTotal(j.at("estimatedTotal").get<int64_t>());
        x.setResources(j.at("resources").get<std::vector<MapGenerator::Resource>>());
    }

    inline void to_json(json & j, const MapGenerator::ResourceSet & x) {
        j = json::object();
        j["estimatedTotal"] = x.getEstimatedTotal();
        j["resources"] = x.getResources();
    }

    inline void from_json(const json & j, MapGenerator::ElevationResult& x) {
        x.setAuthenticationResultCode(j.at("authenticationResultCode").get<std::string>());
        x.setBrandLogoUri(j.at("brandLogoUri").get<std::string>());
        x.setCopyright(j.at("copyright").get<std::string>());
        x.setResourceSets(j.at("resourceSets").get<std::vector<MapGenerator::ResourceSet>>());
        x.setStatusCode(j.at("statusCode").get<int64_t>());
        x.setStatusDescription(j.at("statusDescription").get<std::string>());
        x.setTraceId(j.at("traceId").get<std::string>());
    }

    inline void to_json(json & j, const MapGenerator::ElevationResult & x) {
        j = json::object();
        j["authenticationResultCode"] = x.getAuthenticationResultCode();
        j["brandLogoUri"] = x.getBrandLogoUri();
        j["copyright"] = x.getCopyright();
        j["resourceSets"] = x.getResourceSets();
        j["statusCode"] = x.getStatusCode();
        j["statusDescription"] = x.getStatusDescription();
        j["traceId"] = x.getTraceId();
    }
}



#endif //VUT_BP_ELEVATIONRESULT_H
