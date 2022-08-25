
#pragma once
#include "geGL/Program.h"
#include "geGL/Shader.h"
#include "renderer/scene_objects/Shader.h"
#include "renderer/scene_objects/Texture.h"
#include <memory>
#include <renderer/scene_objects/Uniform.h>
#include <renderer/SceneObject.h>
#include <vector>
#include <map>


namespace MapGenerator::Renderer::SceneObjects {

    class Program : public SceneObject {
    public:
        Program();
        Program(const std::string& name);
        Program(const std::string& name, const std::vector<std::shared_ptr<Shader>>& shaders);
        Program(const std::string& name, const std::shared_ptr<Shader>& vertex, const std::shared_ptr<Shader>& fragment);
        ~Program();

        void addShader(std::shared_ptr<Shader> shader);

        void addTexture(std::shared_ptr<Texture> texture);

        void setTexture(const std::shared_ptr<Texture>& texture, int unit);
        std::vector<std::shared_ptr<Texture>>& getTextures();
        uint getTextureCount();
        std::shared_ptr<Shader> getShader(Shader::Type type);

        std::vector<std::shared_ptr<Shader>> getShaders();

        std::shared_ptr<ge::gl::Program> glProgram();

        void setUniform(std::shared_ptr<Uniform> uniform);

        bool compile();
        void use();
    private:

        struct IntDefaultedToMinusOne{
            int i = -1;

            operator int() const {
                return i;
            }

        };

        bool compiled;
        std::vector<std::shared_ptr<Texture>> additionalTextures;
        std::shared_ptr<Shader> vertexShader;
        std::shared_ptr<Shader> fragmentShader;
        std::shared_ptr<Shader> geometryShader;
        std::shared_ptr<Shader> tessalationControlShader;
        std::shared_ptr<Shader> tessalationEvaluationShader;
        std::shared_ptr<Shader> computeShader;
        std::map<uint, IntDefaultedToMinusOne> uniformLocations;


        std::shared_ptr<ge::gl::Program> program;

    };
} // namespace MapGenerator::Renderer::SceneObjects