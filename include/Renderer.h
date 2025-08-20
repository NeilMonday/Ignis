
#pragma once
#include <vulkan/vulkan.h>
#include <vector>

class Window;

class Renderer {
public:
    Renderer(Window& window);
    ~Renderer();

    void beginFrame();
    void endFrame();
    void waitIdle();

    VkInstance instance() const { return m_instance; }
    VkDevice device() const { return m_device; }
    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }
    VkQueue graphicsQueue() const { return m_graphicsQueue; }
    uint32_t graphicsFamilyIndex() const { return m_graphicsFamily; }
    VkRenderPass renderPass() const { return m_renderPass; }
    VkExtent2D swapchainExtent() const { return m_swapExtent; }
    VkFormat swapchainFormat() const { return m_swapFormat; }
    VkCommandBuffer currentCommandBuffer() const { return m_cmdBuffers[m_currentImage]; }

    VkDescriptorPool imguiDescriptorPool() const { return m_imguiDescriptorPool; }
    VkCommandPool commandPool() const { return m_commandPool; }

private:
    void createInstance();
    void pickPhysicalDevice();
    void createDevice();
    void createSurface();
    void createSwapchain();
    void createImageViews();
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
    void createImguiDescriptorPool();

    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>&);
    VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& caps);

private:
    Window& m_window;
    VkInstance m_instance{};
    VkSurfaceKHR m_surface{};
    VkPhysicalDevice m_physicalDevice{};
    VkDevice m_device{};
    uint32_t m_graphicsFamily{};
    VkQueue m_graphicsQueue{};
    VkSwapchainKHR m_swapchain{};
    std::vector<VkImage> m_swapImages;
    std::vector<VkImageView> m_swapImageViews;
    VkFormat m_swapFormat{};
    VkExtent2D m_swapExtent{};
    VkRenderPass m_renderPass{};
    std::vector<VkFramebuffer> m_framebuffers;
    VkCommandPool m_commandPool{};
    std::vector<VkCommandBuffer> m_cmdBuffers;
    VkSemaphore m_imageAvailable{};
    VkSemaphore m_renderFinished{};
    VkFence m_inFlight{};
    VkDescriptorPool m_imguiDescriptorPool{};
    uint32_t m_currentImage = 0;
    bool m_frameBegun = false;
};
