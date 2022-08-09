#include "renderer/SceneObject.h"
#include <memory>
#include <renderer/scene_objects/Program.h>

using namespace MapGenerator::Renderer::SceneObjects;

Program::Program() : SceneObject() {
}
Program::Program(const std::string& name) : SceneObject(name) {
}

Program::~Program() {
}

void Program::attachShader(std::shared_ptr<Shader> shader) {
    switch (shader->getType()) {
    case Shader::None:
        throw std::runtime_error("Shader type is None");
        break;
    case Shader::Vertex:
        this->vertexShader = shader;
        break;
    case Shader::Fragment:
        this->fragmentShader = shader;
        break;
    case Shader::Geometry:
        this->geometryShader = shader;
        break;
    case Shader::TessalationControl:
        this->tessalationControlShader = shader;
        break;
    case Shader::TessalationEvaluation:
        this->tessalationEvaluationShader = shader;
        break;
    case Shader::Compute:
        this->computeShader = shader;
        break;
    }
}


std::shared_ptr<Shader> Program::getShader(Shader::Type type) {
    switch (type) {
    case Shader::None:
        throw std::runtime_error("Shader type is None");
        break;
    case Shader::Vertex:
        return this->vertexShader;
        break;
    case Shader::Fragment:
        return this->fragmentShader;
        break;
    case Shader::Geometry:
        return this->geometryShader;
        break;
    case Shader::TessalationControl:
        return this->tessalationControlShader;
        break;
    case Shader::TessalationEvaluation:
        return this->tessalationEvaluationShader;
        break;
    case Shader::Compute:
        return this->computeShader;
        break;
    }
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
    return this->program->getLinkStatus();
}
std::shared_ptr<ge::gl::Program> Program::glProgram() {
    return this->program;
}
