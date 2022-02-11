//
// Created by tomas on 21.01.22.
//

#include <renderer/Scene3D.h>
#include <Helper.h>

namespace MapGenerator {
    Scene3D::Scene3D(const shared_ptr<Scene> &scene, const shared_ptr<ge::gl::Context> &ctx,
                     const shared_ptr<Camera> &camera) {
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
            if (program == nullptr) {
                continue;
            }
            //Setup textures - sets up all textures for the model and binds them to the correct texture units
            useTextures(modelId);
            //Setup uniforms (right now only view and projection are supported)
            if (program->getUniformLocation("view") != -1) {
                program->setMatrix4fv("view", glm::value_ptr(view));
            }
            if (program->getUniformLocation("projection") != -1) {
                program->setMatrix4fv("projection", glm::value_ptr(projection));
            }
            if(program->getUniformLocation("lightPos") != -1) {
                program->set3v("lightPos", glm::value_ptr(glm::vec3(0, 1.0f, 0.25)));
            }
            auto drawCount = useModel(modelId);
            auto origProgram = scene->getProgram(scene->getProgramForModel(modelId));
            auto drawMode = GL_TRIANGLES;
            if(origProgram->tessControlShader != -1 && origProgram->tessEvaluationShader != -1) {
                gl->glPatchParameteri(GL_PATCH_VERTICES, 3);
                drawMode = GL_PATCHES;
            }
            //Draw the model
            gl->glDrawElements(drawMode, drawCount, GL_UNSIGNED_INT, nullptr);

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
        auto vertexData = model->vertices;
        auto vertices = std::make_shared<ge::gl::Buffer>(vertexData.size() * sizeof(float), vertexData.data());
        auto indexData = model->indices;
        auto indices = std::make_shared<ge::gl::Buffer>(indexData.size() * sizeof(unsigned int), indexData.data());

        buffers.push_back(vertices);
        buffers.push_back(indices);
        auto vao = std::make_shared<ge::gl::VertexArray>();
        vao->bind();

        vao->addAttrib(vertices, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
        vao->addAttrib(vertices, 1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
        vao->addAttrib(vertices, 2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float));
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
            auto texture = getTexture(modelTextures[i], scene->getTexture(modelTextures[i]));
            texture->bind(i);
        }
    }

    std::shared_ptr<ge::gl::Program> Scene3D::useProgram(int modelId) {
        auto programId = scene->getProgramForModel(modelId);
        //The program exists - use it
        if (programs.find(programId) != programs.end()) {
            programs[programId]->use();
            return programs[programId];
        }
        //Let's grab the shaders and crate the program
        auto program = scene->getProgram(programId);
        if(program == nullptr){
            return nullptr;
        }
        std::vector<std::shared_ptr<ge::gl::Shader>> shaderObjects;
        for(auto shaderId : program->getShaders()){
            auto shader = getShader(shaderId);
            if(shader == nullptr){
                continue;
            }
            shaderObjects.push_back(shader);
        }
        auto programObject = std::make_shared<ge::gl::Program>();
        programObject->attachShaders(shaderObjects);
        programObject->link();
        programs[programId] = programObject;
        programObject->use();
        return programObject;
    }


    std::shared_ptr<ge::gl::Texture> Scene3D::getTexture(int id, const std::shared_ptr<Texture> &tex) {
        if (textures.find(id) != textures.end()) {
            return textures[id];
        }
        auto texture = std::make_shared<ge::gl::Texture>();
        texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA, 0, tex->getWidth(), tex->getHeight());
        texture->bind(GL_TEXTURE_2D);
        texture->setData2D(tex->getData(), GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_TEXTURE_2D, 0, 0, tex->getWidth(),
                           tex->getHeight());
        //texture->generateMipmap();
        texture->texParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
        texture->texParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
        texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        texture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        textures[id] = texture;

        return texture;
    }

    std::shared_ptr<ge::gl::Shader> Scene3D::getShader(int id) {
        if(id == -1) {
            return nullptr;
        }
        if (shaders.find(id) != shaders.end()) {
            return shaders[id];
        }
        auto shader = scene->getShader(id);
        auto shaderType = shader->getType();
        auto shaderSource = shader->getSource();
        auto geGLShaderType = 0;
        switch(shaderType) {
            case Shader::VERTEX:
                geGLShaderType = GL_VERTEX_SHADER;
                break;
            case Shader::FRAGMENT:
                geGLShaderType = GL_FRAGMENT_SHADER;
                break;
            case Shader::GEOMETRY:
                geGLShaderType = GL_GEOMETRY_SHADER;
                break;
            case Shader::COMPUTE:
                geGLShaderType = GL_COMPUTE_SHADER;
                break;
            case Shader::TESS_CONTROL:
                geGLShaderType = GL_TESS_CONTROL_SHADER;
                break;
            case Shader::TESS_EVALUATION:
                geGLShaderType = GL_TESS_EVALUATION_SHADER;
                break;
        }
        auto shaderObj = std::make_shared<ge::gl::Shader>(geGLShaderType, shaderSource);
        shaders[id] = shaderObj;
        return shaderObj;
    }

    Scene3D::~Scene3D() = default;

}

