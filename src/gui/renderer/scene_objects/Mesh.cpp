#include <renderer/scene_objects/Mesh.h>
#include <glm/geometric.hpp>
#include <memory>
#include <geGL/geGL.h>

using namespace MapGenerator::Renderer::SceneObjects;

Mesh::Mesh() : SceneObject() {

}

Mesh::Mesh(const std::string& name) : SceneObject(name) {

}


Mesh::~Mesh() {
}

void Mesh::addVertex(const glm::vec3& vertex) {
    addVertex(vertex, zero3, zero2, zero3);
}

void Mesh::addVertex(const glm::vec3& vertex, const glm::vec3& normal) {
    addVertex(vertex, normal, zero2, zero3);
}

void Mesh::addVertex(const glm::vec3& vertex, const glm::vec3& normal, const glm::vec2& texCoord) {
    addVertex(vertex, normal, texCoord, zero3);
}

void Mesh::addVertex(const glm::vec3& vertex, const glm::vec3& normal, const glm::vec2& texCoord, const glm::vec3& tangent) {
    vertices.push_back(vertex.x);
    vertices.push_back(vertex.y);
    vertices.push_back(vertex.z);
    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);
    vertices.push_back(texCoord.x);
    vertices.push_back(texCoord.y);
    vertices.push_back(tangent.x);
    vertices.push_back(tangent.y);
    vertices.push_back(tangent.z);
}

void Mesh::addIndex(unsigned int index) {
    indices.push_back(index);
}

void Mesh::upload(){
    vertexBuffer = std::make_shared<ge::gl::Buffer>(vertices.size() * sizeof(float), vertices.data());
    indexBuffer = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(unsigned int), indices.data());
    vao = std::make_shared<ge::gl::VertexArray>();
    vao->bind();

    // Position
    vao->addAttrib(vertexBuffer, 0, 3, GL_FLOAT, stride * sizeof(float), 0);
    // Normal
    vao->addAttrib(vertexBuffer, 1, 3, GL_FLOAT, stride * sizeof(float), 3 * sizeof(float));
    // TexCoord
    vao->addAttrib(vertexBuffer, 2, 2, GL_FLOAT, stride * sizeof(float), 6 * sizeof(float));
    // Tangent
    vao->addAttrib(vertexBuffer, 3, 3, GL_FLOAT, stride * sizeof(float), 8 * sizeof(float));

    vao->addElementBuffer(indexBuffer);
};

void Mesh::bind() {
    if(vao != nullptr){
        vao->bind();
        return;
    }

    upload();
    vao->bind();

}

uint Mesh::size() {
    return indices.size();
}

