//
// Created by tomas on 07.01.22.
//

#include "Scene.h"

#include <utility>
#include <algorithm>

namespace MapGenerator {

    Scene::Scene() {

    }

    int Scene::addShader(std::shared_ptr<Shader> shader) {
        int id = generateId(shaders);
        shaders[id] = shader;
        return id;
    }

    int Scene::addTexture(std::shared_ptr<Texture> texture) {
        int id = generateId(textures);
        textures[id] = texture;
        return id;
    }

    int Scene::addModel(std::shared_ptr<Model> model) {
        int id = generateId(models);
        models[id] = model;
        return id;
    }


    int Scene::createProgram(std::shared_ptr<Program> program) {
        int id = generateId(programs);
        programs[id] = program;
        return id;
    }

    void Scene::bindProgram(int id, int modelId) {
        if (programs.find(id) == programs.end()) {
            throw std::runtime_error("Program with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(modelId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " does not exist");
        }
        if (std::find(modelToProgram[modelId].begin(), modelToProgram[modelId].end(), id) != modelToProgram[modelId].end()) {
            throw std::runtime_error("Program with id " + std::to_string(id) + " is already bound to a model");
        }
        modelToProgram[modelId].push_back(id);
    }

    void Scene::bindTexture(int id, int programId) {
        if (textures.find(id) == textures.end()) {
            throw std::runtime_error("Texture with id " + std::to_string(id) + " does not exist");
        }
        if (programs.find(programId) == programs.end()) {
            throw std::runtime_error("Program with id " + std::to_string(programId) + " does not exist");
        }

        programToTexture[programId].push_back(id);
    }

    template<typename T>
    int Scene::generateId(std::map<int, T> &map) {
        static int id = 0;
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
        modelToProgram.erase(modelId);
    }


    void Scene::unbindTexture(int id, int programId) {
        if (textures.find(id) == textures.end()) {
            throw std::runtime_error("Texture with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(programId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(programId) + " does not exist");
        }
        if (programToTexture.find(programId) == programToTexture.end()) {
            throw std::runtime_error("Program with id " + std::to_string(programId) + " is not bound to a texture");
        }
        auto &tex = programToTexture[programId];
        auto it = std::find(tex.begin(), tex.end(), id);
        if (it == tex.end()) {
            throw std::runtime_error("Model with id " + std::to_string(programId) + " is not bound to texture with id " +
                                     std::to_string(id));
        }
        tex.erase(it);
    }

    std::map<int, std::shared_ptr<Model>> Scene::getModels() {
        return models;
    }

    std::vector<int> Scene::getTexturesForProgram(int modelId) {
        if (programToTexture.find(modelId) == programToTexture.end()) {
            return {};
        }
        return programToTexture[modelId];
    }

    std::shared_ptr<Texture> Scene::getTexture(int id) {
        if (textures.find(id) == textures.end()) {
            return nullptr;
        }
        return textures[id];
    }

    std::shared_ptr<Model> Scene::getModel(int id) {
        if (models.find(id) == models.end()) {
            return nullptr;
        }
        return models[id];
    }

    std::vector<int> Scene::getProgramsForModel(int modelId) {
        if (modelToProgram.find(modelId) == modelToProgram.end()) {
            return {};
        }
        return modelToProgram[modelId];
    }

    std::shared_ptr<Program> Scene::getProgram(int programId) {
        if (programs.find(programId) == programs.end()) {
            return nullptr;
        }
        return programs[programId];
    }

    std::shared_ptr<Shader> Scene::getShader(int id) {
        if (shaders.find(id) == shaders.end()) {
            return nullptr;
        }
        return shaders[id];
    }


}

