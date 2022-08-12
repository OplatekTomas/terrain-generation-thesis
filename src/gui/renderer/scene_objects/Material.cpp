#include "Material.h"
#include <memory>
#include <vector>

using namespace MapGenerator::Renderer::SceneObjects;

Material::Material() : SceneObject() {

}

Material::Material(const std::string& name) : SceneObject(name) {

}

Material::~Material() {

}

void Material::setDiffuseTexture(std::shared_ptr<Texture> texture) {
    diffuseTexture = texture;
}

void Material::setNormalTexture(std::shared_ptr<Texture> texture) {
    normalTexture = texture;
}

void Material::setSpecularTexture(std::shared_ptr<Texture> texture) {
    specularTexture = texture;
}

void Material::setMetallicTexture(std::shared_ptr<Texture> texture) {
    metallicTexture = texture;
}

void Material::setRoughnessTexture(std::shared_ptr<Texture> texture) {
    roughnessTexture = texture;
}

std::shared_ptr<Texture> Material::getDiffuseTexture() const {
    return diffuseTexture;
}

std::shared_ptr<Texture> Material::getNormalTexture() const {
    return normalTexture;
}

std::shared_ptr<Texture> Material::getSpecularTexture() const {
    return specularTexture;
}

std::shared_ptr<Texture> Material::getMetallicTexture() const {
    return metallicTexture;
}

std::shared_ptr<Texture> Material::getRoughnessTexture() const {
    return roughnessTexture;
}

void Material::setDiffuseValue(const glm::vec3& value) {
    diffuseValue = value;
}

void Material::setNormalValue(const glm::vec3& value) {
    normalValue = value;
}

void Material::setSpecularValue(const glm::vec3& value) {
    specularValue = value;
}

void Material::setMetallicValue(const float value) {
    metallicValue = value;
}

void Material::setRoughnessValue(const float value) {
    roughnessValue = value;
}

glm::vec3 Material::getDiffuseValue() const {
    return diffuseValue;
}

glm::vec3 Material::getNormalValue() const {
    return normalValue;
}

glm::vec3 Material::getSpecularValue() const {
    return specularValue;
}

float Material::getMetallicValue() const {
    return metallicValue;
}

float Material::getRoughnessValue() const {
    return roughnessValue;
}

std::vector<std::shared_ptr<Texture>> Material::getTextures() const {
    std::vector<std::shared_ptr<Texture>> textures;
    if (diffuseTexture) {
        textures.push_back(diffuseTexture);
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    if (normalTexture) {
        textures.push_back(normalTexture);
    }
    if (specularTexture) {
        textures.push_back(specularTexture);
    }
    if (metallicTexture) {
        textures.push_back(metallicTexture);
    }
    if (roughnessTexture) {
        textures.push_back(roughnessTexture);
    }

    return textures;
}
