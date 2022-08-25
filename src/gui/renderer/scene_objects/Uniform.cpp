#include "Uniform.h"

namespace MapGenerator::Renderer::SceneObjects{


    Uniform::Uniform(const std::string& name) : SceneObject(name) {
    }

    Uniform::Uniform(const std::string& name, const std::any& value, Type type) : SceneObject(name) {
        this->value = value;
        this->type = type;
    }

    Uniform::~Uniform() {
    }
    void Uniform::setValue(const std::any& value) {
        this->value = value;
    }

    void Uniform::setType(Type type) {
        this->type = type;
    }

    std::any Uniform::getValue() const {
        return value;
    }

    Uniform::Type Uniform::getType() const {
        return type;
    }
} // namespace MapGenerator::Renderer::SceneObjects