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
                const shared_ptr<Camera> &camera, int gBufferId);

        ~Scene3D();

        int draw(int height, int width, double scale);

    private:

        shared_ptr<ge::gl::Context> gl;
        shared_ptr<Scene> scene;
        std::shared_ptr<Camera> camera;
        //Keep these two lines in this fucking order, or you will get segfaults when releasing the scene
        std::vector<shared_ptr<ge::gl::Buffer>> buffers;
        map<int, shared_ptr<ge::gl::VertexArray>> models;
        map<int, shared_ptr<ge::gl::Texture>> textures;
        map<int, shared_ptr<ge::gl::Texture>> textureArrays;
        map<int, shared_ptr<ge::gl::Program>> programs;
        map<int, int> drawCounts;
        map<int, shared_ptr<ge::gl::Shader>> shaders;
        map<int, shared_ptr<ge::gl::Framebuffer>> frameBuffers;
        map<int, shared_ptr<ge::gl::Renderbuffer>> renderBuffers;
        map<int, shared_ptr<ge::gl::Buffer>> instances;
        map<int, std::vector<float>> instanceRandomData;
        int gBufferId;
        //Uniform random value generator
        std::mt19937 rng;
        std::uniform_real_distribution<float> dist;

        shared_ptr<ge::gl::Texture> getTexture(int id, const shared_ptr<Texture> &texture, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

        shared_ptr<ge::gl::Shader> getShader(int id);

        void useTextures(int programId);

        int useModel(int id);

        shared_ptr<ge::gl::Program> useProgram(int modelId);

        void drawToScreen(const shared_ptr<Program> &program, int drawCount);

        void drawToTexture(const shared_ptr<Program> &program, int programId, int drawCount);

        void checkForErrors();

        shared_ptr<ge::gl::Texture> getTextureArray(int id, shared_ptr<TextureArray> arr);

        void setupUniforms(int progId, int modelId);


        int cullInstances(int modelId);
    };
}

