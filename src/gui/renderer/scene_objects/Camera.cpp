#include "Camera.h"
#include "renderer/SceneObject.h"
#include "renderer/input/InputHandler.h"
#include <tuple>

namespace MapGenerator::Renderer::SceneObjects {
    Camera::Camera() : SceneObject() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::vec3(1.0f, 0.0f, 0.0f);
        yaw = -90.0f;
        pitch = 0.0f;
        movementSpeed = 0.01;
        mouseSensitivity = 0.01;
        setPerspective(60.0f, 0.1f, 100.f);
        setDimensions(1.0f, 1.0f);
    }

    Camera::Camera(const std::string& name) : SceneObject(name) {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        yaw = -90.0f;
        pitch = 0.0f;
        movementSpeed = 0.001;
        mouseSensitivity = 0.01;
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

    void Camera::setMouseSensitivity(float sensitivity) {
        this->mouseSensitivity = sensitivity;
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
        auto [deltaX, deltaY] = std::make_tuple(0, 0);
        if (input->isMouseButtonPressed(InputHandler::MOUSE_LEFT)) {
            if (std::get<0>(previousPoint) == 0 && std::get<1>(previousPoint) == 0) {
                viewMatrix = glm::lookAt(position, position + front, up);
                previousPoint = input->getAbsolutePosition();
                std::cout << "new point" << std::endl;
                return;
            }
            auto [x, y] = input->getAbsolutePosition();
            deltaX = x - std::get<0>(previousPoint);
            deltaY = y - std::get<1>(previousPoint);
            previousPoint = input->getAbsolutePosition();
            std::cout << "deltaX: " << deltaX << " deltaY: " << deltaY << std::endl;

        } else {
            previousPoint = std::make_tuple(0, 0);
        }
        // This can be moved inside the if statement, but the frametime is more consistent if it is here
        yaw -= deltaX * mouseSensitivity * deltaTime;
        pitch -= deltaY * mouseSensitivity * deltaTime;
        pitch = pitch > 89.0f ? 89.0f : pitch < -89.0f ? -89.0f
                                                       : pitch;

        glm::vec3 frontTmp;
        frontTmp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frontTmp.y = sin(glm::radians(pitch));
        frontTmp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = -glm::normalize(frontTmp);
        // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
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

    glm::vec3 Camera::getPosition() const {
        return position;
    }

    float Camera::getWidth() const {
        return width;
    }

    float Camera::getHeight() const {
        return height;
    }

    void Camera::setFOV(float fov) {
        this->setPerspective(fov, this->near, this->far);
    }

    std::tuple<float, float> Camera::getRotation() const {
        return std::make_tuple(yaw, pitch);
    }

    float Camera::getMovementSpeed() const {
        return movementSpeed;
    }

    float Camera::getMouseSensitivity() const {
        return mouseSensitivity;
    }
    
    bool Camera::acceptsInput() const{
        return this->acceptingInput;
    }

    float Camera::getFOV() const {
        return fov;
    }


} // namespace MapGenerator::Renderer::SceneObjects