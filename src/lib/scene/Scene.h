//
// Created by tomas on 07.01.22.
//

#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <scene/Model.h>
#include <scene/Texture.h>
#include <stdexcept>

namespace MapGenerator{
    class Scene {
    public:
        Scene();

        int addModel(std::shared_ptr<Model> model);
        int addTexture(std::shared_ptr<Texture> texture);
        int addVertexShader(std::string shader);
        int addFragmentShader(std::string shader);
        void bindTexture(int id, int modelId);
        void bindProgram(int id, int modelId);
        int createProgram(int vertexShaderId, int fragmentShaderId);
        void unbindProgram(int id, int modelId);
        void unbindTexture(int id, int modelId);


    private:
        template<typename T> int generateId(std::map<int, T> &map);

        std::map<int, std::shared_ptr<class Model>> models;
        std::map<int, std::shared_ptr<class Texture>> textures;
        std::map<int, std::vector<int>> model_to_texture;
        std::map<int, int> model_to_program;
        std::map<int, std::string> fragment_shaders;
        std::map<int, std::string> vertex_shaders;
        std::map<int, std::tuple<int, int>> programs;
    };
};



