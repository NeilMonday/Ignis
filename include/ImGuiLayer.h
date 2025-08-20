
#pragma once
#include <vulkan/vulkan.h>

class Window;
class Renderer;

class ImGuiLayer {
public:
    ImGuiLayer(Window& window, Renderer& renderer);
    ~ImGuiLayer();

    void begin();
    void end(VkCommandBuffer cmd);

private:
    Window& m_window;
    Renderer& m_renderer;
    bool m_initialized = false;
};
