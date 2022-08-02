//
// Created by tomas on 25.01.22.
//


#pragma once


#include <string>

namespace MapGenerator {
    class Shader {

    public:
        enum ShaderType {
            VERTEX,
            FRAGMENT,
            GEOMETRY,
            TESS_CONTROL,
            TESS_EVALUATION,
            COMPUTE
        };

        Shader(std::string code, ShaderType type, std::string name);

        std::string getSource() {
            return shaderCode;
        }

        ShaderType getType() {
            return type;
        }

    private:
        std::string shaderCode;
        ShaderType type;
        std::string name;
    };
}


