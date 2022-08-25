
#pragma once

#include "renderer/scene_objects/Material.h"
#include "renderer/scene_objects/Program.h"
#include <algorithm>
#include <geGL/geGL.h>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <renderer/SceneObject.h>
#include <renderer/scene_objects/Camera.h>
#include <renderer/scene_objects/Mesh.h>
#include <renderer/scene_objects/Uniform.h>
#include <string>
#include <vector>

namespace MapGenerator::Renderer::SceneObjects {
    class Node : public SceneObject {
    public:
        Node();
        Node(const std::string& name);
        Node(const std::string& name, std::shared_ptr<Node> parent);

        void draw();

        ~Node();

        void addChild(std::shared_ptr<Node> child);
        void removeChild(std::shared_ptr<Node> child);
        void removeAllChildren();
        void setParent(std::shared_ptr<Node> parent);
        void setTransform(const glm::mat4& transform);
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setScale(const glm::vec3& scale);
        void setMesh(std::shared_ptr<Mesh> mesh);
        void setMaterial(std::shared_ptr<Material> material);
        void setProgram(std::shared_ptr<Program> program);
        void setCamera(std::shared_ptr<Camera> camera);
        void addUniform(std::shared_ptr<Uniform> uniform);

        glm::mat4& getTransform();
        glm::vec3& getPosition();
        glm::vec3& getRotation();
        glm::vec3& getScale();
        std::shared_ptr<Node> getParent() const;
        std::vector<std::shared_ptr<Node>> getChildren() const;
        std::shared_ptr<Mesh> getMesh() const;
        std::shared_ptr<Material> getMaterial() const;
        std::shared_ptr<Program> getProgram() const;
        std::shared_ptr<Camera> getCamera() const;

    private:
        void useCamera();

        std::vector<std::shared_ptr<Uniform>> uniforms;

        std::vector<std::shared_ptr<Node>> children;
        std::shared_ptr<Node> parent;
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        std::shared_ptr<Program> program;
        std::shared_ptr<Camera> camera;

        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4x4 transform;
    };

} // namespace MapGenerator::Renderer::SceneObjects
