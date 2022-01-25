//
// Created by tomas on 21.01.22.
//

#include <renderer/Scene3D.h>
#include <Helper.h>

namespace MapGenerator {
    Scene3D::Scene3D(const std::shared_ptr<Scene>& scene, std::shared_ptr<ge::gl::Context> ctx,
                     std::shared_ptr<Camera> camera) {
        this->scene = scene;
        this->gl = ctx;
        this->camera = camera;
    }


    int Scene3D::draw(int height, int width) {
        auto view = camera->getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float) width / (float) height, 0.005f, 100.0f);
        for (auto &modelPair: scene->getModels()) {
            auto modelId = modelPair.first;
            //Setup shaders/program
            auto program = useProgram(modelId);
            //Setup textures - sets up all textures for the model and binds them to the correct texture units
            useTextures(modelId);
            //Setup uniforms (right now only view and projection are supported)
            program->setMatrix4fv("view", glm::value_ptr(view));
            program->setMatrix4fv("projection", glm::value_ptr(projection));
            auto drawCount = useModel(modelId);
            //Draw the model
            gl->glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, nullptr);

        }

        return 0;
    }

    int Scene3D::useModel(int id) {
        auto model = scene->getModel(id);
        if (models.find(id) != models.end()) {
            auto vao = models[id];
            vao->bind();
            return model->indices.size();
        }
        auto vertices = std::make_shared<ge::gl::Buffer>(model->vertices.size() * sizeof(float), model->vertices.data(),
                                                         GL_STATIC_DRAW);
        auto indices = std::make_shared<ge::gl::Buffer>(model->indices.size() * sizeof(int), model->indices.data(),
                                                        GL_STATIC_DRAW);

        buffers.push_back(vertices);
        buffers.push_back(indices);
        auto vao = std::make_shared<ge::gl::VertexArray>();
        vao->bind();

        vao->addAttrib(vertices, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
        vao->addAttrib(vertices, 1, 2, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
        vao->addAttrib(vertices, 2, 3, GL_FLOAT, 8 * sizeof(float), 5 * sizeof(float));
        vao->addElementBuffer(indices);
        models[id] = vao;
        vao->bind();
        return model->indices.size();
    }

    void Scene3D::useTextures(int modelId) {
        auto modelTextures = scene->getTexturesForModel(modelId);
        if (modelTextures.empty()) {
            return;
        }
        for (size_t i = 0; i < modelTextures.size(); i++) {
            auto texture = getTexture(modelTextures[i], *scene->getTexture(modelTextures[i]));
            texture->bind(i);
        }
    }

    std::shared_ptr<ge::gl::Program> Scene3D::useProgram(int modelId) {
        auto program = scene->getProgramForModel(modelId);
        //The program exists - use it
        if (programs.find(program) != programs.end()) {
            programs[program]->use();
            return programs[program];
        }
        //Let's grab the shaders and crate the program
        auto[vShader, fShader] = scene->getProgram(program);
        auto vertexShader = getShader(vShader);
        auto fragShader = getShader(fShader);
        auto programObject = std::make_shared<ge::gl::Program>(vertexShader, fragShader);
        programs[program] = programObject;
        programObject->use();
        return programObject;
    }


    std::shared_ptr<ge::gl::Texture> Scene3D::getTexture(int id, const Texture &tex) {
        if (textures.find(id) != textures.end()) {
            return textures[id];
        }

        auto texture = std::make_shared<ge::gl::Texture>();
        texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA, 0, tex.getWidth(), tex.getHeight());
        texture->bind(GL_TEXTURE_2D);
        texture->setData2D(tex.getData(), GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_TEXTURE_2D, 0, 0, tex.getWidth(),
                           tex.getHeight());
        //texture->generateMipmap();
        texture->texParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
        texture->texParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
        texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        texture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        textures[id] = texture;

        return texture;
    }

    std::shared_ptr<ge::gl::Shader> Scene3D::getShader(int id) {
        if (shaders.find(id) != shaders.end()) {
            return shaders[id];
        }
        auto shader = scene->getShader(id);
        auto shaderType = shader->getType();
        auto shaderSource = shader->getSource();
        auto geGLShaderType = shaderType == Shader::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
        auto shaderObj = std::make_shared<ge::gl::Shader>(geGLShaderType, shaderSource);
        shaders[id] = shaderObj;
        return shaderObj;
    }

}

