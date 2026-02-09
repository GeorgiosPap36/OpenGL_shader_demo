#ifndef BASIC_CAMERA_CONTROLLER_H
#define BASIC_CAMERA_CONTROLLER_H

#include <algorithm>

#include "camera.h"

class BasicCameraController : public Camera {
    public:
    BasicCameraController(glm::vec3 up, glm::vec3 forward, glm::vec3 position, float pitch, float yaw, float fovY) 
        : Camera(up, forward, position, pitch, yaw, fovY) {

    }

    void update(float dt, std::map<int, bool>& keyboard, glm::vec2 mouseMovement) override {
        glm::vec3 movement;
        // X
        if (keyboard[GLFW_KEY_D]) {
            movement.x = movementSpeed;
        } else if (keyboard[GLFW_KEY_A]) {
            movement.x = -movementSpeed;
        }
        // Y
        if (keyboard[GLFW_KEY_SPACE]) {
            movement.y = movementSpeed;
        } else if (keyboard[GLFW_KEY_LEFT_SHIFT]) {
            movement.y = -movementSpeed;
        }
        // Z
        if (keyboard[GLFW_KEY_W]) {
            movement.z = -movementSpeed;
        } else if (keyboard[GLFW_KEY_S]) {
            movement.z = movementSpeed;
        }
        
        // Mouse movement
        applyYawChange(mouseMovement.x * mouseSensitivity);
        applyPitchChange(mouseMovement.y * mouseSensitivity);
        updateForwardVector();

        position += (forward * movement.z) + (up * movement.y) + (right() * movement.x);

        // std::cout << "Yaw: " << yaw << ", Pitch: " << pitch << std::endl;
        // std::cout << "Forward: " << forward.to_string() << ", Up: " << up.to_string() << ", Right: " << right().to_string() << std::endl;
    }

    private:
    float mouseSensitivity = 1;
    float movementSpeed = 1;

    void applyYawChange(float val) {
        yaw += val;
    }

    void applyPitchChange(float val) {
        pitch += val;
        pitch = std::clamp(pitch, -89.0f, 89.0f);
    }

    void updateForwardVector() {
        float radPitch = pitch * PI / 180.0;
        float radYaw = yaw * PI / 180.0;

        float cosPitch = cos(radPitch);
        float sinPitch = sin(radPitch);
        float cosYaw = cos(radYaw);
        float sinYaw = sin(radYaw);

        forward = glm::vec3(-sinYaw * cosPitch, sinPitch, -cosPitch * cosYaw);
    }
};

#endif