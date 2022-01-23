//
// Created by tomas on 21.01.22.
//


#pragma once

#include <memory>
#include <geGL/geGL.h>
#include <scene/Scene.h>
#include <renderer/Camera.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
namespace MapGenerator{
    class Camera;
    class Scene3D {
    public:
        Scene3D(std::shared_ptr<Scene> scene, std::shared_ptr<ge::gl::Context> ctx, std::shared_ptr<Camera> camera);
        void prepareScene();
        int draw(int height, int width);
    private:
        shared_ptr<ge::gl::Context> gl;
        shared_ptr<Scene> scene;
        std::shared_ptr<Camera> camera;
        map<int, shared_ptr<ge::gl::VertexArray>> models;
        map<int, shared_ptr<ge::gl::Texture>> textures;
        map<int, shared_ptr<ge::gl::Program>> programs;
        map<int, shared_ptr<ge::gl::Shader>> fragShaders;
        map<int, shared_ptr<ge::gl::Shader>> vertShaders;

        shared_ptr<ge::gl::VertexArray> getModel(int id, const Model &model);
        shared_ptr<ge::gl::Texture> getTexture(int id, const Texture &texture);

        void useTextures(int modelId);
    };
}

