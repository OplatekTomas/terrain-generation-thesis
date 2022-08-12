
#pragma once
#include "renderer/scene_objects/Node.h"
#include <geGL/geGL.h>
#include <iostream>
#include <memory>
#include <renderer/SceneObject.h>
#include <renderer/scene_objects/Mesh.h>
#include <renderer/scene_objects/Program.h>
#include <renderer/scene_objects/Shader.h>
#include <renderer/scene_objects/Texture.h>
#include <renderer/scene_objects/Camera.h>
#include <vector>
#include <renderer/input/InputHandler.h>

using namespace MapGenerator::Renderer::SceneObjects;


namespace MapGenerator::Renderer {


    class Scene : public SceneObject {
    public:
        Scene();
         ~Scene();


        void setRoot(std::shared_ptr<Node> parent);
        void setInputHandler(std::shared_ptr<InputHandler> inputHandler);
        std::shared_ptr<Node> getRoot();
        void setDimensions(int width, int height);
        void build();

        void draw();

    private:
        void addMesh(std::shared_ptr<Mesh> mesh);
        void addProgram(std::shared_ptr<Program> program);
        void addShader(std::shared_ptr<Shader> shader);
        void addTexture(std::shared_ptr<Texture> texture);
        void addMaterial(std::shared_ptr<Material> material);
        void addCamera(std::shared_ptr<Camera> camera);

        void buildNode(const Node& node);

        void drawNode(Node& node);

        std::shared_ptr<InputHandler> inputHandler;

        std::shared_ptr<Camera> currentCamera;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        std::shared_ptr<Node> root;
        std::set<std::shared_ptr<Mesh>> meshes;
        std::set<std::shared_ptr<Texture>> textures;
        std::set<std::shared_ptr<Program>> programs;
        std::set<std::shared_ptr<Shader>> shaders;
        std::set<std::shared_ptr<Material>> materials;
        std::set<std::shared_ptr<Camera>> cameras;
    };

} // namespace MapGenerator::Renderer
