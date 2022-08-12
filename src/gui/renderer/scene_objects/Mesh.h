
#pragma once

#include <geGL/geGL.h>
#include <memory>
#include <renderer/SceneObject.h>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace MapGenerator::Renderer::SceneObjects {
    class Mesh : public SceneObject {
    public:
        Mesh();
        Mesh(const std::string& name);
        ~Mesh();

        void addVertex(const glm::vec3& vertex);
        void addVertex(const glm::vec3& vertex, const glm::vec3& normal);
        void addVertex(const glm::vec3& vertex, const glm::vec3& normal, const glm::vec2& texCoord);
        void addVertex(const glm::vec3& vertex, const glm::vec3& normal, const glm::vec2& texCoord, const glm::vec3& tangent);
        void addIndex(unsigned int index);

        void bind();

        void upload();

        uint size();

        void calculateTangents();
        void calculateNormals();


    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        glm::vec3 zero3 = glm::vec3(0.0f);
        glm::vec2 zero2 = glm::vec2(0.0f);

        std::shared_ptr<ge::gl::VertexArray> vao;
        std::shared_ptr<ge::gl::Buffer> vertexBuffer;
        std::shared_ptr<ge::gl::Buffer> indexBuffer;

        int stride = 11;

    };
} // namespace MapGenerator::Renderer::SceneObjects
