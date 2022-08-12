#include "Camera.h"
#include "renderer/SceneObject.h"
#include "renderer/input/InputHandler.h"

namespace MapGenerator::Renderer::SceneObjects {
    Camera::Camera() : SceneObject() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::vec3(1.0f, 0.0f, 0.0f);
        yaw = 0.0f;
        pitch = 0.0f;
        movementSpeed = 0.001;
        setPerspective(60.0f, 0.1f, 100.f);
        setDimensions(1.0f, 1.0f);
    }

    Camera::Camera(const std::string& name) : SceneObject(name) {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        yaw = 0.0f;
        pitch = 0.0f;
        movementSpeed = 0.001;
        setPerspective(60.0f, 0.1f, 100.f);
        setDimensions(1.0f, 1.0f);
    }

    Camera::~Camera() {
    }

    void Camera::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void Camera::setRotation(float yaw, float pitch) {
        this->yaw = yaw;
        this->pitch = pitch;
    }

    void Camera::setMovementSpeed(float speed) {
        this->movementSpeed = speed;
    }

    void Camera::setWorldUp(const glm::vec3& worldUp) {
        this->worldUp = worldUp;
    }

    void Camera::setInputHandler(std::shared_ptr<InputHandler> input) {
        this->input = input;
    }

    void Camera::acceptInput(bool acceptingInput) {
        this->acceptingInput = acceptingInput;
    }

    void Camera::update(float deltaTime) {
        if (!acceptingInput || input == nullptr) {
            return;
        }
        if (input->isKeyPressed(InputHandler::KEY_W)) {
            position += front * movementSpeed * deltaTime;
        }
        if (input->isKeyPressed(InputHandler::KEY_S)) {
            position -= front * movementSpeed * deltaTime;
        }
        if (input->isKeyPressed(InputHandler::KEY_A)) {
            position -= right * movementSpeed * deltaTime;
        }
        if (input->isKeyPressed(InputHandler::KEY_D)) {
            position += right * movementSpeed * deltaTime;
        }

        viewMatrix = glm::lookAt(position, position + front, up);

    }

    void Camera::setDimensions(float width, float height) {
        this->width = width;
        this->height = height;
        this->aspect = width / height;
        this->projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    void Camera::setPerspective(float fov, float near, float far) {
        this->fov = fov;
        this->near = near;
        this->far = far;
        this->projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    glm::mat4 Camera::getViewMatrix() const {
        return viewMatrix;
    }

    glm::mat4 Camera::getProjectionMatrix() const {
        return projectionMatrix;
    }

    float Camera::getWidth() const {
        return width;
    }

    float Camera::getHeight() const {
        return height;
    }

} // namespace MapGenerator::Renderer::SceneObjects