//
// Created by tomas on 04.10.21.
//

#pragma once

#include <nlohmann/json.hpp>

namespace MapGenerator{

    using namespace nlohmann;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    template<typename T>
    inline std::shared_ptr<T> get_optional(const json &j, const char *property) {
        if (j.find(property) != j.end()) {
            auto result = j.at(property).get<T>();
            auto ptr = std::make_shared<T>();
            *ptr = result;
            return ptr;
        }
        return std::shared_ptr<T>();
    }

    template<typename T>
    inline std::shared_ptr<T> get_optional(const json &j, std::string property) {
        return get_optional<T>(j, property.data());
    }

}
