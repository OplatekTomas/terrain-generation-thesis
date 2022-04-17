//
// Created by tomas on 07.04.22.
//

#include <renderer/Skybox.h>
#include <StaticFunctions.h>
#include <shaders_gui/Shaders.h>
#include <glm/glm.hpp>
#include <Logger.h>
#include <glm/gtc/type_ptr.hpp>

namespace MapGenerator {


    Skybox::Skybox(std::shared_ptr<ge::gl::Context> gl, std::shared_ptr<Camera> camera, float width, float height) {
        this->gl = gl;
        this->camera = camera;
        this->width = width;
        this->height = height;
        std::string path = "renderer/textures/";
        auto extension = ".png";
        std::vector<std::string> faces;
        faces.push_back(path + "right" + extension);
        faces.push_back(path + "left" + extension);
        faces.push_back(path + "up" + extension);
        faces.push_back(path + "down" + extension);
        faces.push_back(path + "back" + extension);
        faces.push_back(path + "front" + extension);
        loadCubeMap(faces);
        loadVertices();
        loadShaders();
    }

    void Skybox::loadCubeMap(std::vector<std::string> textures) {
        texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_CUBE_MAP, GL_RGBA, 0, 512, 512);
        //texture->bind(GL_TEXTURE_CUBE_MAP);
        gl->glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getId());
        checkError();
        for (int i = 0; i < 6; i++) {

            auto data = readPng(textures[i]);
            gl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             data.data());
            //texture->setData2D((unsigned char *)data.data(), GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, width, height);
            checkError();

        }

        texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        texture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        texture->texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture->texParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        texture->texParameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void Skybox::loadVertices() {
        vao = std::make_shared<ge::gl::VertexArray>();
        vbo = std::make_shared<ge::gl::Buffer>(GL_ARRAY_BUFFER);
        vao->bind();
        vbo->bind(GL_ARRAY_BUFFER);
        vbo->setData(this->skyboxVertices, 108 * sizeof(float));
        vao->addAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
    }

    void Skybox::loadShaders() {
        vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, GUIShaders::getSkyboxVS());
        fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, GUIShaders::getSkyboxFS());
        program = std::make_shared<ge::gl::Program>(vs, fs);
    }

    void Skybox::checkError() {
        GLenum error = gl->glGetError();
        if (error != GL_NO_ERROR) {
            Logger::log("GL_ERROR in sky rendering" + std::to_string(error));
        }
    }

    void Skybox::draw(int frameBuffer) {
        gl->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        auto projection = glm::perspective(glm::radians(60.0f), (float) width / (float) height, 0.005f, 100.0f);
        auto view = camera->getViewMatrix();
        gl->glDepthFunc(GL_LEQUAL);
        vao->bind();
        program->use();
        program->setMatrix4fv("projection", glm::value_ptr(projection));
        program->setMatrix4fv("view", glm::value_ptr(view));
        program->set3f("cameraPos", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

        gl->glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getId());
        gl->glDrawArrays(GL_TRIANGLES, 0, 36);
        gl->glDepthFunc(GL_LESS);
        gl->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    }

    void Skybox::setDimensions(float width, float height) {
        this->width = width;
        this->height = height;
    }


}