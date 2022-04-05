//
// Created by tomas on 28.03.22.
//


#pragma once

#include <memory>
#include <scene/Model.h>
#include <filesystem>
#include <vector>
#include <assimp/mesh.h>

namespace fs = std::filesystem;

namespace MapGenerator{
    class ObjectLoader {
    public:
        static std::vector<std::shared_ptr<Model>> load(const std::string &path);
        static std::vector<std::shared_ptr<Model>> loadAll(const std::vector<std::string> &path);
    private:
        static float findScale(const std::vector<Vertex> &vertices);
        std::vector<Vertex> readVertices(const aiMesh *mesh);
    };
}





