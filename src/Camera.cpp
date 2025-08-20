
#include "Camera.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <algorithm>
void Camera::update(float dt, Input& input, bool rmbHeld) {
    glm::vec3 f = forward();
    glm::vec3 r = glm::normalize(glm::cross(f, worldUp));
    glm::vec3 u = worldUp;
    float speed = moveSpeed * dt;
    if (input.keyDown(GLFW_KEY_W)) position += f * speed;
    if (input.keyDown(GLFW_KEY_S)) position -= f * speed;
    if (input.keyDown(GLFW_KEY_A)) position -= r * speed;
    if (input.keyDown(GLFW_KEY_D)) position += r * speed;
    if (input.keyDown(GLFW_KEY_Q)) position -= u * speed;
    if (input.keyDown(GLFW_KEY_E)) position += u * speed;
    if (rmbHeld) {
        yaw   += static_cast<float>(input.mouseDeltaX()) * lookSensitivity;
        pitch -= static_cast<float>(input.mouseDeltaY()) * lookSensitivity;
        pitch = std::clamp(pitch, -89.0f, 89.0f);
    }
}
