#include "renderer/SceneObject.h"
#include <functional>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <renderer/scene_objects/Program.h>
#include <string>

using namespace MapGenerator::Renderer::SceneObjects;

Program::Program() : SceneObject() {
    compiled = false;
}
Program::Program(const std::string& name) : SceneObject(name) {
    compiled = false;
}

Program::Program(const std::string& name, const std::vector<std::shared_ptr<Shader>>& shaders) : SceneObject(name) {
    compiled = false;
    for (auto shader : shaders) {
        addShader(shader);
    }
}

Program::Program(const std::string& name, const std::shared_ptr<Shader>& vertex, const std::shared_ptr<Shader>& fragment) : SceneObject(name) {
    if (vertex->getType() != Shader::Type::VERTEX) {
        throw std::runtime_error("Vertex shader is not of type VERTEX");
    }
    if (fragment->getType() != Shader::Type::FRAGMENT) {
        throw std::runtime_error("Fragment shader is not of type FRAGMENT");
    }
    this->vertexShader = vertex;
    this->fragmentShader = fragment;
    compiled = false;
}

Program::~Program() {
}

void Program::addShader(std::shared_ptr<Shader> shader) {
    switch (shader->getType()) {
    case Shader::None:
        throw std::runtime_error("Shader type is None");
        break;
    case Shader::VERTEX:
        this->vertexShader = shader;
        break;
    case Shader::FRAGMENT:
        this->fragmentShader = shader;
        break;
    case Shader::GEOMETRY:
        this->geometryShader = shader;
        break;
    case Shader::TESSALATION_CTRL:
        this->tessalationControlShader = shader;
        break;
    case Shader::TESSALATION_EVAL:
        this->tessalationEvaluationShader = shader;
        break;
    case Shader::COMPUTE:
        this->computeShader = shader;
        break;
    }
}

std::shared_ptr<Shader> Program::getShader(Shader::Type type) {
    switch (type) {
    case Shader::None:
        throw std::runtime_error("Shader type is None");
        break;
    case Shader::VERTEX:
        return this->vertexShader;
        break;
    case Shader::FRAGMENT:
        return this->fragmentShader;
        break;
    case Shader::GEOMETRY:
        return this->geometryShader;
        break;
    case Shader::TESSALATION_CTRL:
        return this->tessalationControlShader;
        break;
    case Shader::TESSALATION_EVAL:
        return this->tessalationEvaluationShader;
        break;
    case Shader::COMPUTE:
        return this->computeShader;
        break;
    }
    return nullptr;
}

bool Program::compile() {
    if (this->vertexShader == nullptr) {
        throw std::runtime_error("Vertex shader is not set");
    }
    if (this->fragmentShader == nullptr) {
        throw std::runtime_error("Fragment shader is not set");
    }
    this->program = std::make_shared<ge::gl::Program>();

    this->program->attachShaders({this->vertexShader->glShader(), this->fragmentShader->glShader()});
    if (this->geometryShader != nullptr) {
        this->program->attachShaders(this->geometryShader->glShader());
    }
    if (this->tessalationControlShader != nullptr) {
        this->program->attachShaders(this->tessalationControlShader->glShader());
    }
    if (this->tessalationEvaluationShader != nullptr) {
        this->program->attachShaders(this->tessalationEvaluationShader->glShader());
    }
    if (this->computeShader != nullptr) {
        this->program->attachShaders(this->computeShader->glShader());
    }
    this->program->link();
    compiled = this->program->getLinkStatus();

    auto uniforms = program->getInfo()->uniforms;
    for (auto uniform : uniforms) {
        auto name = std::string(uniform.first);
        auto location = program->getUniformLocation(name);
        this->uniformLocations[std::hash<std::string>{}(name)] = {location};
    }
    return compiled;
}
std::shared_ptr<ge::gl::Program> Program::glProgram() {
    return this->program;
}

void Program::setUniform(std::shared_ptr<Uniform> uniform) {
    // TODO cache uniform locations on successful compile
    auto id = uniform->getId();
    auto location = this->uniformLocations[id];
    if (location == -1) {
        std::cerr << "Uniform " << name << " not found" << std::endl;
        return;
    }
    switch (uniform->getType()) {
    case Uniform::Type::BOOL:
        gl->glProgramUniform1i(this->program->getId(), location, std::any_cast<bool>(uniform->getValue()));
        break;
    case Uniform::Type::INT:
        gl->glProgramUniform1i(program->getId(), location, std::any_cast<int>(uniform->getValue()));
        break;
    case Uniform::Type::FLOAT:
        gl->glProgramUniform1f(program->getId(), location, std::any_cast<float>(uniform->getValue()));
        break;
    case Uniform::Type::VEC2:
        gl->glProgramUniform2fv(program->getId(), location, 1, glm::value_ptr(std::any_cast<glm::vec2>(uniform->getValue())));
        break;
    case Uniform::Type::VEC3:
        gl->glProgramUniform3fv(program->getId(), location, 1, glm::value_ptr(std::any_cast<glm::vec3>(uniform->getValue())));
        break;
    case Uniform::Type::VEC4:
        gl->glProgramUniform4fv(program->getId(), location, 1, glm::value_ptr(std::any_cast<glm::vec4>(uniform->getValue())));
        break;
    case Uniform::Type::MAT3:
        gl->glProgramUniformMatrix3fv(program->getId(), location, 1, GL_FALSE, glm::value_ptr(std::any_cast<glm::mat3>(uniform->getValue())));
        break;
    case Uniform::Type::MAT4:
        gl->glProgramUniformMatrix4fv(program->getId(), location, 1, GL_FALSE, glm::value_ptr(std::any_cast<glm::mat4>(uniform->getValue())));
        break;
    }
}

void Program::setTexture(const std::shared_ptr<Texture>& texture, int unit) {
    auto id = texture->getId();
    auto location = this->uniformLocations[id];
    if (location == -1) {
        std::cerr << "Texture " << texture->getName() << " not found" << std::endl;
        return;
    }
    gl->glUniform1i(location, unit);
    //checkError();
}

std::vector<std::shared_ptr<Shader>> Program::getShaders() {
    std::vector<std::shared_ptr<Shader>> shaders;
    if (this->vertexShader != nullptr) {
        shaders.push_back(this->vertexShader);
    }
    if (this->fragmentShader != nullptr) {
        shaders.push_back(this->fragmentShader);
    }
    if (this->geometryShader != nullptr) {
        shaders.push_back(this->geometryShader);
    }
    if (this->tessalationControlShader != nullptr) {
        shaders.push_back(this->tessalationControlShader);
    }
    if (this->tessalationEvaluationShader != nullptr) {
        shaders.push_back(this->tessalationEvaluationShader);
    }
    if (this->computeShader != nullptr) {
        shaders.push_back(this->computeShader);
    }
    return shaders;
}

void Program::addTexture(std::shared_ptr<Texture> texture) {
    this->additionalTextures.push_back(texture);
}




std::vector<std::shared_ptr<Texture>>& Program::getTextures() {
    return this->additionalTextures;
}
void Program::use() {
    if (!compiled) {
        auto success = compile();
        if (!success) {
            auto err = this->program->getInfoLog();
            throw std::runtime_error("Program could not be compiled (" + err + ")");
        }
    }
    this->program->use();
}

uint Program::getTextureCount() {
    return this->additionalTextures.size();
}

