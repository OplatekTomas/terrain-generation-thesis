
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <renderer/SceneObject.h>
#include <renderer/input/InputHandler.h>
#include <tuple>

namespace MapGenerator::Renderer::SceneObjects{

    class Camera : public SceneObject {
    public:


        Camera();
        Camera(const std::string& name);
        ~Camera();

        void setPosition(const glm::vec3& position);
        void setRotation(float yaw, float pitch);
        void setMovementSpeed(float speed);
        void setMouseSensitivity(float sensitivity);
        void setFOV(float fov);
        void setPerspective(float fov, float near, float far);
        void setDimensions(float width, float height);
        void setWorldUp(const glm::vec3& worldUp);
    
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        glm::vec3 getPosition() const;
        std::tuple<float, float> getRotation() const;
        float getWidth() const;
        float getHeight() const;
        float getFOV() const;
        float getMovementSpeed() const;
        float getMouseSensitivity() const;
        bool acceptsInput() const;


        void setInputHandler(std::shared_ptr<InputHandler> input);
        void acceptInput(bool acceptingInput);
        void update(float deltaTime);

       


    private:
        std::shared_ptr<InputHandler> input;
        bool acceptingInput = false;

        std::tuple<int, int> previousPoint = std::make_tuple(0, 0);

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
        float mouseSensitivity;
        float fov;
        float near;
        float far;
        float width;
        float height;
        float aspect;
    };
}



