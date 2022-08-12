
#pragma once

#include "geGL/Generated/OpenGLTypes.h"
#include "geGL/Shader.h"
#include <memory>
#include <renderer/SceneObject.h>
#include <string>

namespace MapGenerator::Renderer::SceneObjects {

    class Shader : public SceneObject {
    public:
        enum Type {
            None,
            VERTEX,
            FRAGMENT,
            GEOMETRY,
            TESSALATION_CTRL,
            TESSALATION_EVAL,
            COMPUTE
        };

        Shader();
        Shader(const std::string& source, Type type);
        Shader(const std::string& source, const std::string& name, Type type);
        void setSource(const std::string& source);
        void setType(Type type);

        std::string getInfoLog();
        std::string& getSource();
        void recompile();
        std::shared_ptr<ge::gl::Shader> glShader();
        Type getType();
        std::string getTypeString();
        ~Shader();

    private:
        GLenum getGLType();
        std::shared_ptr<ge::gl::Shader> shader = nullptr;
        std::string infoLog;
        std::string code;

        Type type;
    };
} // namespace MapGenerator::Renderer::SceneObjects