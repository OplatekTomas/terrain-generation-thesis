//
// Created by tomas on 07.01.22.
//

#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <scene/Model.h>
#include <scene/Shader.h>
#include <scene/Texture.h>
#include <scene/Program.h>
#include <scene/TextureArray.h>
#include <stdexcept>

namespace MapGenerator{
    class Scene {
    public:
        Scene();

        int addModel(std::shared_ptr<Model> model);
        void removeModel(int id);
        int addTexture(std::shared_ptr<Texture> texture);
        int addShader(std::shared_ptr<Shader> shader);
        void bindTexture(int id, int programId);
        void bindProgram(int id, int modelId);
        int addProgram(std::shared_ptr<Program> program);
        void unbindProgram(int id, int modelId);
        void unbindTexture(int id, int programId);
        int addTextureArray(const std::shared_ptr<TextureArray>& textureArray);
        void bindTextureArray(int id, int programId);


        std::map<int, std::shared_ptr<Model>> getModels();
        std::vector<int> getTexturesForProgram(int modelId);
        std::vector<int> getTextureArraysForProgram(int modelId);
        std::shared_ptr<TextureArray> getTextureArray(int id);
        std::shared_ptr<Texture> getTexture(int id);
        std::shared_ptr<Model> getModel(int id);
        std::vector<int> getProgramsForModel(int modelId);
        std::shared_ptr<Program> getProgram(int id);
        std::shared_ptr<Shader> getShader(int id);
    private:
        template<typename T> int generateId(std::map<int, T> &map);
        int itemId = 0;

        std::map<int, std::shared_ptr<class Model>> models;
        std::map<int, std::shared_ptr<class Texture>> textures;
        std::map<int, std::vector<int>> programToTexture;
        std::map<int, std::vector<int>> modelToProgram;
        std::map<int, std::shared_ptr<class Shader>> shaders;
        std::map<int, std::shared_ptr<class Program>> programs;
        std::map<int, std::shared_ptr<class TextureArray>> textureArrays;
        std::map<int, std::vector<int>> programToTextureArray;
    };
};



