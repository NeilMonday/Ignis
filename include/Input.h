
#pragma once
#include <GLFW/glfw3.h>

class Input {
public:
    explicit Input(GLFWwindow* window) : m_window(window) { glfwGetCursorPos(m_window, &m_lastX, &m_lastY); }
    void beginFrame() {
        double x, y; glfwGetCursorPos(m_window, &x, &y);
        m_deltaX = x - m_lastX; m_deltaY = y - m_lastY; m_lastX = x; m_lastY = y;
    }
    bool keyDown(int key) const { return glfwGetKey(m_window, key) == GLFW_PRESS; }
    bool mouseDown(int btn) const { return glfwGetMouseButton(m_window, btn) == GLFW_PRESS; }
    double mouseDeltaX() const { return m_deltaX; }
    double mouseDeltaY() const { return m_deltaY; }
private:
    GLFWwindow* m_window{};
    double m_lastX{0.0}, m_lastY{0.0}, m_deltaX{0.0}, m_deltaY{0.0};
};
