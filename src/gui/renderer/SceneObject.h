
#pragma once

#include <geGL/geGL.h>
#include <memory>

namespace MapGenerator::Renderer {
    class SceneObject {
    public:
        SceneObject();
        SceneObject(std::string name);
        ~SceneObject();
        //virtual void render() = 0;
        std::string getName();
        uint getId();

        void setName(std::string name);

        static void setFrameTime(float frameTime);
        static void init(std::shared_ptr<ge::gl::Context> ctx);
    
        inline static bool initialized = false;

    protected:
        std::string name;
        inline static float frameTime = 0.0f;
        std::string getNextDefaultName();

        void checkError();

        inline static std::shared_ptr<ge::gl::Context> gl = nullptr;

    private:
        uint id = 0;

        virtual void ignorethisshitrighthere(){};

        inline static unsigned int defaultNameCounter = 0;
    };
} // namespace MapGenerator::Renderer::SceneObjects
