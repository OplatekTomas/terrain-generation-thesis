
#pragma once
#include <geGL/geGL.h>
#include <iostream>
#include <memory>
#include <renderer/SceneObject.h>
#include <renderer/scene_objects/Program.h>
#include <renderer/scene_objects/Shader.h>
#include <renderer/scene_objects/Texture.h>
#include <renderer/scene_objects/Mesh.h>
#include <vector>


using namespace MapGenerator::Renderer::SceneObjects;


namespace MapGenerator::Renderer {


    class Scene : public SceneObject {
    public:
        Scene();
        ~Scene();

        void render();

        void addMesh(std::shared_ptr<Mesh> mesh);
        void addProgram(std::shared_ptr<Program> program);
        void addShader(std::shared_ptr<Shader> shader);
        void addTexture(std::shared_ptr<Texture> texture);
        void bindProgramToMesh(Program& program, Mesh& mesh);
        void bindTextureToProgram(const Texture& texture, const Program& program);
        void bindTextureToProgram(const Texture& texture, const Program& program, std::string uniformName);
        void bindTextureToProgram(const Texture& texture, const Program& program, std::string uniformName, int textureUnit);

    private:
        std::vector<std::shared_ptr<Mesh>> geometry;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Program>> programs;
        std::vector<std::shared_ptr<Shader>> shaders;
    };

} // namespace MapGenerator::Renderer
