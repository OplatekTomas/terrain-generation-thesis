#include "Shader.h"
#include "geGL/Shader.h"
#include <memory>

using namespace MapGenerator::Renderer::SceneObjects;

Shader::Shader() : SceneObject() {
    this->type = Type::None;
}

Shader::~Shader() {
}

Shader::Shader(const std::string& source, Shader::Type type) : SceneObject() {
    this->type = type;
    this->code = source;
}

Shader::Shader(const std::string& source, const std::string& name, Shader::Type type) : SceneObject(name) {
    this->type = type;
    this->code = source;
}

void Shader::setSource(const std::string& source) {
    this->code = source;
}

void Shader::setType(Shader::Type type) {
    this->type = type;
}

void Shader::getSource(std::string& source) {
    source = this->code;
}

Shader::Type Shader::getType() {
    return this->type;
}

std::shared_ptr<ge::gl::Shader> Shader::glShader() {
    if (this->shader != nullptr) {
        return this->shader;
    }
    this->shader = std::make_shared<ge::gl::Shader>(this->code, this->type);
    this->shader->compile();
    infoLog = this->shader->getInfoLog();
    return this->shader;
}
