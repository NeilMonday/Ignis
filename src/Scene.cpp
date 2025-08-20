
#include "Scene.h"

GameObject& Scene::create(const std::string& name) {
    m_objects.push_back({name});
    return m_objects.back();
}
