#include "Node.h"
#include "geGL/Generated/OpenGLConstants.h"
#include <glm/gtc/type_ptr.hpp>

using namespace MapGenerator::Renderer::SceneObjects;

Node::Node() : SceneObject() {
}

Node::Node(const std::string& name) : SceneObject(name) {
}

Node::Node(const std::string& name, std::shared_ptr<Node> parent) : SceneObject(name) {
    this->parent = parent;
}

Node::~Node() {
}

void Node::addChild(std::shared_ptr<Node> child) {
    children.push_back(child);
}

void Node::removeChild(std::shared_ptr<Node> child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Node::removeAllChildren() {
    children.clear();
}

void Node::setParent(std::shared_ptr<Node> parent) {
    this->parent = parent;
}

void Node::setTransform(const glm::mat4& transform) {
    this->transform = transform;
}

void Node::setPosition(const glm::vec3& position) {
    this->translation = position;
}

void Node::setRotation(const glm::vec3& rotation) {
    this->rotation = rotation;
}

void Node::setScale(const glm::vec3& scale) {
    this->scale = scale;
}

void Node::setMesh(std::shared_ptr<Mesh> mesh) {
    this->mesh = mesh;
}

void Node::setMaterial(std::shared_ptr<Material> material) {
    this->material = material;
}

void Node::setProgram(std::shared_ptr<Program> program) {
    this->program = program;
}

void Node::setCamera(std::shared_ptr<Camera> camera) {
    this->camera = camera;
}

void Node::addUniform(std::shared_ptr<Uniform> uniform) {
    this->uniforms.push_back(uniform);
}

glm::mat4& Node::getTransform() {
    return transform;
}

glm::vec3& Node::getPosition() {
    return translation;
}

glm::vec3& Node::getRotation() {
    return rotation;
}

glm::vec3& Node::getScale() {
    return scale;
}

std::shared_ptr<Node> Node::getParent() const {
    return parent;
}

std::vector<std::shared_ptr<Node>> Node::getChildren() const {
    return children;
}

std::shared_ptr<Mesh> Node::getMesh() const {
    return mesh;
}

std::shared_ptr<Material> Node::getMaterial() const {
    return material;
}

std::shared_ptr<Program> Node::getProgram() const {
    return program;
}

std::shared_ptr<Camera> Node::getCamera() const {
    return camera;
}

void Node::useCamera() { //TODO change to more modern way

    if (camera == nullptr) {
        return;
    }
    gl->glViewport(0, 0, camera->getWidth(), camera->getHeight());

    //rework as uniforms
    if (program->glProgram()->getUniformLocation("view") != -1) {
        program->glProgram()->setMatrix4fv("view", glm::value_ptr(camera->getViewMatrix()));
    }
    if (program->glProgram()->getUniformLocation("projection") != -1) {
        program->glProgram()->setMatrix4fv("projection", glm::value_ptr(camera->getProjectionMatrix()));
    }
}

void Node::addTexture(std::shared_ptr<Texture> texture) {
    textures.push_back(texture);
}


void Node::draw() {
    if (mesh == nullptr) {
        return;
    }
    if (program == nullptr) {
        return;
    }

    program->use();
    for (auto& uniform : uniforms) {
        program->setUniform(uniform);
    }
    useCamera();
    auto i = program->getTextureCount();
    for (auto& texture : textures) {
        texture->bind(i);
        program->setTexture(texture, i);
        i++;
    }
    mesh->bind();
    gl->glDrawElements(GL_TRIANGLES, mesh->size(), GL_UNSIGNED_INT, 0);
}
