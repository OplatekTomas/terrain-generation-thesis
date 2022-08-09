#include <renderer/Scene.h>

using namespace MapGenerator::Renderer;

Scene::Scene() : SceneObject() {
}

Scene::~Scene() {
}

void Scene::render() {

}

void Scene::addMesh(std::shared_ptr<Mesh> mesh) {
    geometry.push_back(mesh);
}

void Scene::addProgram(std::shared_ptr<Program> program) {
    programs.push_back(program);
}

void Scene::addShader(std::shared_ptr<Shader> shader) {

}


void Scene::addTexture(std::shared_ptr<Texture> texture) {
}

void Scene::bindProgramToMesh(Program& program, Mesh& mesh) {
}

void Scene::bindTextureToProgram(const Texture& texture, const Program& program) {
}

void Scene::bindTextureToProgram(const Texture& texture, const Program& program, std::string uniformName) {
}

void Scene::bindTextureToProgram(const Texture& texture, const Program& program, std::string uniformName, int textureUnit) {
}
