#include <renderer/Scene.h>

using namespace MapGenerator::Renderer;

Scene::Scene() : SceneObject() {
}

Scene::~Scene() {
}

void Scene::setInputHandler(std::shared_ptr<InputHandler> inputHandler) {
    this->inputHandler = inputHandler;
    for(auto c : this->cameras) {
        c->setInputHandler(inputHandler);
    }
}

void Scene::addMesh(std::shared_ptr<Mesh> mesh) {
    this->meshes.insert(mesh);
}

void Scene::addProgram(std::shared_ptr<Program> program) {
    this->programs.insert(program);
}

void Scene::addShader(std::shared_ptr<Shader> shader) {
    this->shaders.insert(shader);
}

void Scene::addTexture(std::shared_ptr<Texture> texture) {
    this->textures.insert(texture);
}

void Scene::addMaterial(std::shared_ptr<Material> material) {
    this->materials.insert(material);
}

void Scene::addCamera(std::shared_ptr<Camera> camera) {
    this->cameras.insert(camera);
}

void Scene::setRoot(std::shared_ptr<Node> parent) {
    root = parent;
}

std::shared_ptr<Node> Scene::getRoot() {
    return root;
}

void Scene::buildNode(const Node& node) {
    if (node.getMesh() != nullptr) {
        addMesh(node.getMesh());
    }
    if (node.getProgram() != nullptr) {
        auto program = node.getProgram();
        for (auto shader : program->getShaders()) {
            addShader(shader);
        }
        for (auto texture : program->getTextures()) {
            addTexture(texture);
        }
        addProgram(program);
        addProgram(node.getProgram());
    }
    if (node.getMaterial() != nullptr) {
        auto material = node.getMaterial();
        addMaterial(material);
    }
    if (node.getCamera() != nullptr) {
        auto camera = node.getCamera();
        camera->setInputHandler(inputHandler);
        addCamera(camera);
    }
    for (auto child : node.getChildren()) {
        buildNode(*child);
    }
}

void Scene::build() {
    buildNode(*root);
}

void Scene::drawNode(Node& node) {
    node.draw();
    for (auto child : node.getChildren()) {
        drawNode(*child);
    }
}

void Scene::draw() {
    for(auto camera: this->cameras){
        camera->update(this->frameTime);
    }

    drawNode(*root);
}

void Scene::setDimensions(int width, int height){
    for(auto camera: this->cameras){
        camera->setDimensions(width, height);
    }
}

