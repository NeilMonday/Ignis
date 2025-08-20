
#pragma once
#include <string>
#include <vulkan/vulkan.h>
struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    GLFWwindow* handle() const { return m_window; }
    VkSurfaceKHR createSurface(VkInstance instance);

    int width() const { return m_width; }
    int height() const { return m_height; }
    bool shouldClose() const;
    void pollEvents();

private:
    GLFWwindow* m_window = nullptr;
    int m_width{};
    int m_height{};
};
