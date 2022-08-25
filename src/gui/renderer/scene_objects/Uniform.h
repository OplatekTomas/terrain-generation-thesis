
#pragma once

#include <any>
#include <memory>
#include <renderer/SceneObject.h>

namespace MapGenerator::Renderer::SceneObjects {

    class Uniform : public SceneObject {
    public:

        enum Type{
            FLOAT,
            INT,
            BOOL,
            VEC2,
            VEC3,
            VEC4,
            MAT3,
            MAT4
        };
        Uniform(const std::string& name);
        Uniform(const std::string& name, const std::any& value, Type type);
        void setValue(const std::any& value);
        void setType(Type type);
        
        std::any getValue() const;
        Type getType() const;
        ~Uniform();

    private:
        Type type;
        std::any value;
    };
} // namespace MapGenerator::Renderer::SceneObjects
