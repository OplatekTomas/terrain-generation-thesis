#include "Shader.h"
#include "geGL/Generated/OpenGLTypes.h"
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

Shader::Shader(const std::string& name, const std::string& source, Shader::Type type) : SceneObject(name) {
    this->type = type;
    this->code = source;
}

void Shader::setSource(const std::string& source) {
    this->code = source;
}

void Shader::setType(Shader::Type type) {
    this->type = type;
}

std::string Shader::getInfoLog() {
    return this->infoLog;
}

std::string& Shader::getSource() {
    return this->code;
}

Shader::Type Shader::getType() {
    return this->type;
}

std::string Shader::getTypeString() {
    switch (type) {
    case Type::None:
        return "None";
    case Type::VERTEX:
        return "VERTEX";
    case Type::FRAGMENT:
        return "FRAGMENT";
    case Type::GEOMETRY:
        return "GEOMETRY";
    case Type::TESSALATION_CTRL:
        return "TESSALATION_CTRL";
    case Type::TESSALATION_EVAL:
        return "TESSALATION_EVAL";
    case Type::COMPUTE:
        return "COMPUTE";
    default:
        return "";
    }
}

void Shader::recompile() {
    shader->setSource(this->code);
    this->shader->compile();
    infoLog = this->shader->getInfoLog();
}

std::shared_ptr<ge::gl::Shader> Shader::glShader() {
    if (this->shader != nullptr) {
        return this->shader;
    }
    this->shader = std::make_shared<ge::gl::Shader>(getGLType(), this->code);
    this->shader->compile();
    infoLog = this->shader->getInfoLog();
    return this->shader;
}

GLenum Shader::getGLType() {
    GLenum type;
    switch (this->type) {
    case None:
        throw std::runtime_error("Shader type is None");
        break;
    case VERTEX:
        type = GL_VERTEX_SHADER;
        break;
    case FRAGMENT:
        type = GL_FRAGMENT_SHADER;
        break;
    case GEOMETRY:
        type = GL_GEOMETRY_SHADER;
        break;
    case TESSALATION_CTRL:
        type = GL_TESS_CONTROL_SHADER;
        break;
    case TESSALATION_EVAL:
        type = GL_TESS_EVALUATION_SHADER;
        break;
    case COMPUTE:
        type = GL_COMPUTE_SHADER;
        break;
    }
    return type;
}
