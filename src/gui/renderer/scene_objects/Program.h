
#pragma once
#include "geGL/Program.h"
#include "geGL/Shader.h"
#include "renderer/scene_objects/Shader.h"
#include <memory>
#include <renderer/SceneObject.h>


namespace MapGenerator::Renderer::SceneObjects {

    class Program : public SceneObject {
    public:
        Program();
        Program(const std::string& name);
        ~Program();

        void attachShader(std::shared_ptr<Shader> shader);
        std::shared_ptr<Shader> getShader(Shader::Type type);

        std::shared_ptr<ge::gl::Program> glProgram();

        bool compile();
        void use();
        void unuse();
    private:
        std::shared_ptr<Shader> vertexShader;
        std::shared_ptr<Shader> fragmentShader;
        std::shared_ptr<Shader> geometryShader;
        std::shared_ptr<Shader> tessalationControlShader;
        std::shared_ptr<Shader> tessalationEvaluationShader;
        std::shared_ptr<Shader> computeShader;


        std::shared_ptr<ge::gl::Program> program;

    };
} // namespace MapGenerator::Renderer::SceneObjects