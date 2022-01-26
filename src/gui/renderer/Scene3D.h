//
// Created by tomas on 21.01.22.
//


#pragma once

#include <memory>
#include <geGL/geGL.h>
#include <scene/Scene.h>
#include <scene/Shader.h>
#include <renderer/Camera.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
namespace MapGenerator {
    class Camera;

    class Scene3D {
    public:
        Scene3D(const shared_ptr<Scene> &scene, const shared_ptr<ge::gl::Context> &ctx,
                const shared_ptr<Camera> &camera);

        ~Scene3D();

        int draw(int height, int width);

    private:


        shared_ptr<ge::gl::Context> gl;
        shared_ptr<Scene> scene;
        std::shared_ptr<Camera> camera;
        //Keep these two lines in this fucking order, or you will get segfaults when releasing the scene
        std::vector<shared_ptr<ge::gl::Buffer>> buffers;
        map<int, shared_ptr<ge::gl::VertexArray>> models;
        map<int, shared_ptr<ge::gl::Texture>> textures;
        map<int, shared_ptr<ge::gl::Program>> programs;
        map<int, shared_ptr<ge::gl::Shader>> shaders;

        shared_ptr<ge::gl::Texture> getTexture(int id, const Texture &texture);

        shared_ptr<ge::gl::Shader> getShader(int id);

        void useTextures(int modelId);

        int useModel(int id);

        shared_ptr<ge::gl::Program> useProgram(int modelId);

    };
}

