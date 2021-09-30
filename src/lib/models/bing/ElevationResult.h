//
// Created by tomas on 30.09.21.
//

#ifndef VUT_BP_ELEVATIONRESULT_H
#define VUT_BP_ELEVATIONRESULT_H


//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Welcome data = nlohmann::json::parse(jsonString);

#pragma once

#include "nlohmann/json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

namespace quicktype {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

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
    void from_json(const json & j, quicktype::Resource & x);
    void to_json(json & j, const quicktype::Resource & x);

    void from_json(const json & j, quicktype::ResourceSet & x);
    void to_json(json & j, const quicktype::ResourceSet & x);

    void from_json(const json & j, quicktype::ElevationResult & x);
    void to_json(json & j, const quicktype::ElevationResult & x);

    inline void from_json(const json & j, quicktype::Resource& x) {
        x.setType(j.at("__type").get<std::string>());
        x.setElevations(j.at("elevations").get<std::vector<int64_t>>());
        x.setZoomLevel(j.at("zoomLevel").get<int64_t>());
    }

    inline void to_json(json & j, const quicktype::Resource & x) {
        j = json::object();
        j["__type"] = x.getType();
        j["elevations"] = x.getElevations();
        j["zoomLevel"] = x.getZoomLevel();
    }

    inline void from_json(const json & j, quicktype::ResourceSet& x) {
        x.setEstimatedTotal(j.at("estimatedTotal").get<int64_t>());
        x.setResources(j.at("resources").get<std::vector<quicktype::Resource>>());
    }

    inline void to_json(json & j, const quicktype::ResourceSet & x) {
        j = json::object();
        j["estimatedTotal"] = x.getEstimatedTotal();
        j["resources"] = x.getResources();
    }

    inline void from_json(const json & j, quicktype::ElevationResult& x) {
        x.setAuthenticationResultCode(j.at("authenticationResultCode").get<std::string>());
        x.setBrandLogoUri(j.at("brandLogoUri").get<std::string>());
        x.setCopyright(j.at("copyright").get<std::string>());
        x.setResourceSets(j.at("resourceSets").get<std::vector<quicktype::ResourceSet>>());
        x.setStatusCode(j.at("statusCode").get<int64_t>());
        x.setStatusDescription(j.at("statusDescription").get<std::string>());
        x.setTraceId(j.at("traceId").get<std::string>());
    }

    inline void to_json(json & j, const quicktype::ElevationResult & x) {
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
