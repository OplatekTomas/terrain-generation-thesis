//
// Created by tomas on 25.01.22.
//

#include <scene/Shader.h>

#include <utility>


MapGenerator::Shader::Shader(std::string code, MapGenerator::Shader::ShaderType type, std::string name) {
    this->type = type;
    this->shaderCode = std::move(code);
    this->name = std::move(name);
}

