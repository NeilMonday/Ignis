
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Input;
class Camera {
public:
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 60.0f;
    float moveSpeed = 5.0f;
    float lookSensitivity = 0.1f;

    glm::mat4 viewMatrix() const {
        glm::vec3 f = forward();
        return glm::lookAt(position, position + f, worldUp);
    }
    glm::mat4 projMatrix(float aspect) const {
        glm::mat4 p = glm::perspective(glm::radians(fov), aspect, 0.01f, 1000.0f);
        p[1][1] *= -1.0f; return p;
    }
    glm::vec3 forward() const {
        float cy = cos(glm::radians(yaw)), sy = sin(glm::radians(yaw));
        float cp = cos(glm::radians(pitch)), sp = sin(glm::radians(pitch));
        return glm::normalize(glm::vec3(cy*cp, sp, sy*cp));
    }
    glm::mat3 rotationMatrix() const {
        glm::vec3 f = forward();
        glm::vec3 r = glm::normalize(glm::cross(f, worldUp));
        glm::vec3 u = glm::normalize(glm::cross(r, f));
        return glm::mat3(r,u,f);
    }
    void update(float dt, Input& input, bool rmbHeld);
    static inline const glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
};
