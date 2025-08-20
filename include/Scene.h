
#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct GameObject {
    std::string name;
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
};

class Scene {
public:
    GameObject& create(const std::string& name);
    std::vector<GameObject>& objects() { return m_objects; }

private:
    std::vector<GameObject> m_objects;
};
