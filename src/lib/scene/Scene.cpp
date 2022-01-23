//
// Created by tomas on 07.01.22.
//

#include "Scene.h"

#include <utility>
#include <algorithm>

namespace MapGenerator {

    Scene::Scene() {

    }

    int Scene::addFragmentShader(std::string shader) {
        int id = generateId(fragmentShaders);
        fragmentShaders[id] = std::move(shader);
        return id;
    }

    int Scene::addVertexShader(std::string shader) {
        int id = generateId(vertexShaders);
        vertexShaders[id] = std::move(shader);
        return id;
    }

    int Scene::addTexture(std::shared_ptr<Texture> texture) {
        int id = generateId(textures);
        textures[id] = std::move(texture);
        return id;
    }

    int Scene::addModel(std::shared_ptr<Model> model) {
        int id = generateId(models);
        models[id] = std::move(model);
        return id;
    }

    int Scene::createProgram(int vertexShaderId, int fragmentShaderId) {
        int id = generateId(programs);
        programs[id] = std::make_tuple(vertexShaderId, fragmentShaderId);
        return id;
    }

    void Scene::bindProgram(int id, int modelId) {
        if (programs.find(id) == programs.end()) {
            throw std::runtime_error("Program with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(modelId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " does not exist");
        }
        if (modelToProgram.find(modelId) != modelToProgram.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is already bound to a program");
        }
        modelToProgram[modelId] = id;
    }

    void Scene::bindTexture(int id, int modelId) {
        if (textures.find(id) == textures.end()) {
            throw std::runtime_error("Texture with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(modelId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " does not exist");
        }
        modelToTexture[modelId].push_back(id);
    }

    template<typename T>
    int Scene::generateId(std::map<int, T> &map) {
        int id = 0;
#pragma omp critical
        {
            while (map.find(id) != map.end()) {
                id++;
            }
        }
        return id;
    }

    void Scene::unbindProgram(int id, int modelId) {
        if (programs.find(id) == programs.end()) {
            throw std::runtime_error("Program with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(modelId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " does not exist");
        }
        if (modelToProgram.find(modelId) == modelToProgram.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is not bound to a program");
        }
        if (modelToProgram[modelId] != id) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is bound to a different program");
        }
        modelToProgram.erase(modelId);
    }



    void Scene::unbindTexture(int id, int modelId) {
        if (textures.find(id) == textures.end()) {
            throw std::runtime_error("Texture with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(modelId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " does not exist");
        }
        if (modelToTexture.find(modelId) == modelToTexture.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is not bound to a texture");
        }
        auto& tex = modelToTexture[modelId];
        auto it = std::find(tex.begin(), tex.end(), id);
        if (it == tex.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is not bound to texture with id " + std::to_string(id));
        }
        tex.erase(it);
    }

    std::map<int, std::shared_ptr<Model>> Scene::getModels() {
        return models;
    }

    std::vector<int> Scene::getTexturesForModel(int modelId) {
        if(modelToTexture.find(modelId) == modelToTexture.end()){
            return {};
        }
        return modelToTexture[modelId];
    }

    std::shared_ptr<Texture> Scene::getTexture(int id) {
        if(textures.find(id) == textures.end()){
            return nullptr;
        }
        return textures[id];
    }


}

