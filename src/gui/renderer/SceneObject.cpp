
#include <renderer/SceneObject.h>

namespace MapGenerator::Renderer {

    SceneObject::SceneObject() {
        this->name = getNextDefaultName();
    }

    SceneObject::SceneObject(std::string name) {
        this->name = name;
    }

    SceneObject::~SceneObject() {
    }
    std::string SceneObject::getName() {
        return name;
    }

    void SceneObject::setName(std::string name) {
        this->name = name;
    }

    void SceneObject::setFrameTime(float frameTime) {
        SceneObject::frameTime = frameTime;
    }

    std::string SceneObject::getNextDefaultName() {
        auto name = "SceneObject_" + std::to_string(SceneObject::defaultNameCounter++);
        return name;
    }

    void SceneObject::init(std::shared_ptr<ge::gl::Context> ctx) {
        SceneObject::gl = ctx;
        SceneObject::initialized = true;
    }

} // namespace MapGenerator::Renderer

uint MapGenerator::Renderer::SceneObject::getId() {
    if (id != 0) {
        return id;
    }
    // Calculate hash of name and set it as id
    auto name = getName();
    id = std::hash<std::string>{}(name);
    return id;
}

void MapGenerator::Renderer::SceneObject::checkError() {
    auto error = gl->glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Error: " << error << std::endl;
        throw std::runtime_error("Error in GL");
    }
}
