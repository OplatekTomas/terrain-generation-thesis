
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <renderer/SceneObject.h>
#include <renderer/input/InputHandler.h>

namespace MapGenerator::Renderer::SceneObjects{

    class Camera : public SceneObject {
    public:


        Camera();
        Camera(const std::string& name);
        ~Camera();

        void setPosition(const glm::vec3& position);
        void setRotation(float yaw, float pitch);
        void setMovementSpeed(float speed);
        
        void setWorldUp(const glm::vec3& worldUp);
    
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        void setInputHandler(std::shared_ptr<InputHandler> input);
        void acceptInput(bool acceptingInput);
        void update(float deltaTime);
        void setPerspective(float fov, float near, float far);
        void setDimensions(float width, float height);

        float getWidth() const;
        float getHeight() const;        


    private:
        std::shared_ptr<InputHandler> input;
        bool acceptingInput = false;

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        float yaw;
        float pitch;
        float movementSpeed;
        float fov;
        float near;
        float far;
        float width;
        float height;
        float aspect;
    };
}



