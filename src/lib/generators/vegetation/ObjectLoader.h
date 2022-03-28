//
// Created by tomas on 28.03.22.
//


#pragma once

#include <memory>
#include <scene/Model.h>

namespace MapGenerator{
    class ObjectLoader {
    public:
        static std::shared_ptr<Model> load(const std::string &path);
    private:
        static float findScale(const std::vector<Vertex> &vertices);
    };
}





