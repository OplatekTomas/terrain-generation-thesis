

#pragma once

#include <renderer/scene_objects/Texture.h>
#include <geGL/geGL.h>
#include <glm/fwd.hpp>
#include <memory>
#include <glm/vec3.hpp>
#include <renderer/SceneObject.h>
#include <vector>

namespace MapGenerator::Renderer::SceneObjects {
    class Material : public SceneObject {
    public:
        Material();
        Material(const std::string& name);

        void setDiffuseTexture(std::shared_ptr<Texture> texture);
        void setNormalTexture(std::shared_ptr<Texture> texture);
        void setSpecularTexture(std::shared_ptr<Texture> texture);
        void setMetallicTexture(std::shared_ptr<Texture> texture);
        void setRoughnessTexture(std::shared_ptr<Texture> texture);

        std::shared_ptr<Texture> getDiffuseTexture() const;
        std::shared_ptr<Texture> getNormalTexture() const;
        std::shared_ptr<Texture> getSpecularTexture() const;
        std::shared_ptr<Texture> getMetallicTexture() const;
        std::shared_ptr<Texture> getRoughnessTexture() const;

        std::vector<std::shared_ptr<Texture>> getTextures() const;

        void setDiffuseValue(const glm::vec3& value);
        void setNormalValue(const glm::vec3& value);
        void setSpecularValue(const glm::vec3& value);
        void setMetallicValue(const float value);
        void setRoughnessValue(const float value);

        glm::vec3 getDiffuseValue() const;
        glm::vec3 getNormalValue() const;
        glm::vec3 getSpecularValue() const;
        float getMetallicValue() const;
        float getRoughnessValue() const;

        ~Material();

    private:
        std::shared_ptr<Texture> diffuseTexture;
        std::shared_ptr<Texture> normalTexture;
        std::shared_ptr<Texture> specularTexture;
        std::shared_ptr<Texture> metallicTexture;
        std::shared_ptr<Texture> roughnessTexture;

        glm::vec3 diffuseValue;
        glm::vec3 normalValue;
        glm::vec3 specularValue;
        float metallicValue;
        float roughnessValue;
    

    };
} // namespace MapGenerator::Renderer::SceneObjects
