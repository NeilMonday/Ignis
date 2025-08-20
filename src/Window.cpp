
#include "Window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height) {
    if(!glfwInit()) throw std::runtime_error("GLFW init failed");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!m_window) throw std::runtime_error("GLFW window creation failed");
}

Window::~Window() {
    if(m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

VkSurfaceKHR Window::createSurface(VkInstance instance) {
    VkSurfaceKHR surface{};
    if(glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create VkSurfaceKHR");
    }
    return surface;
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }
void Window::pollEvents() { glfwPollEvents(); }
