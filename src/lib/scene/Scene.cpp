//
// Created by tomas on 07.01.22.
//

#include "Scene.h"

#include <utility>
#include <algorithm>

namespace MapGenerator {

    int Scene::addFragmentShader(std::string shader) {
        int id = generateId(fragment_shaders);
        fragment_shaders[id] = std::move(shader);
        return id;
    }

    int Scene::addVertexShader(std::string shader) {
        int id = generateId(vertex_shaders);
        vertex_shaders[id] = std::move(shader);
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
        if (model_to_program.find(modelId) != model_to_program.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is already bound to a program");
        }
        model_to_program[modelId] = id;
    }

    void Scene::bindTexture(int id, int modelId) {
        if (textures.find(id) == textures.end()) {
            throw std::runtime_error("Texture with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(modelId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " does not exist");
        }
        model_to_texture[modelId].push_back(id);
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
        if (model_to_program.find(modelId) == model_to_program.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is not bound to a program");
        }
        if (model_to_program[modelId] != id) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is bound to a different program");
        }
        model_to_program.erase(modelId);
    }



    void Scene::unbindTexture(int id, int modelId) {
        if (textures.find(id) == textures.end()) {
            throw std::runtime_error("Texture with id " + std::to_string(id) + " does not exist");
        }
        if (models.find(modelId) == models.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " does not exist");
        }
        if (model_to_texture.find(modelId) == model_to_texture.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is not bound to a texture");
        }
        auto& tex = model_to_texture[modelId];
        auto it = std::find(tex.begin(), tex.end(), id);
        if (it == tex.end()) {
            throw std::runtime_error("Model with id " + std::to_string(modelId) + " is not bound to texture with id " + std::to_string(id));
        }
        tex.erase(it);
    }
}

