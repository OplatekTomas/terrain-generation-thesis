//
// Created by tomas on 07.04.22.
//


#pragma once

#include <string>
#include <vector>
#include <geGL/geGL.h>
#include <memory>
#include <renderer/Camera.h>


namespace MapGenerator {
    class Skybox {


    public:
        explicit Skybox(std::shared_ptr<ge::gl::Context> gl, std::shared_ptr<class Camera> camera, float width, float height);

        void setDimensions(float width, float height);

        void draw();

    private:
        void loadCubeMap(std::vector<std::string> paths);

        void loadVertices();

        void loadShaders();

        std::shared_ptr<ge::gl::Program> program;
        std::shared_ptr<ge::gl::VertexArray> vao;
        std::shared_ptr<ge::gl::Buffer> vbo;
        std::shared_ptr<ge::gl::Texture> texture;
        std::shared_ptr<ge::gl::Shader> vs;
        std::shared_ptr<ge::gl::Shader> fs;
        std::shared_ptr<ge::gl::Context> gl;
        float width, height;
        std::shared_ptr<Camera> camera;

        float skyboxVertices[108] = {
                // positions
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                -1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f
        };


    };
}



