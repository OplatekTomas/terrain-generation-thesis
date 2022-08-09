
#pragma once

#include "geGL/Shader.h"
#include <memory>
#include <renderer/SceneObject.h>
#include <string>

namespace MapGenerator::Renderer::SceneObjects {

    class Shader : public SceneObject {
    public:
        enum Type {
            None,
            Vertex,
            Fragment,
            Geometry,
            TessalationControl,
            TessalationEvaluation,
            Compute
        };

        Shader();
        Shader(const std::string& source, Type type);
        Shader(const std::string& source, const std::string& name, Type type);
        void setSource(const std::string& source);
        void setType(Type type);

        void getSource(std::string& source);

        std::shared_ptr<ge::gl::Shader> glShader();
        Type getType();

        ~Shader();

    private:
        std::shared_ptr<ge::gl::Shader> shader;

        std::string infoLog;

        std::string code;
        Type type;
    };
} // namespace MapGenerator::Renderer::SceneObjects