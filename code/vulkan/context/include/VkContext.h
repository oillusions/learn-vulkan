#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <thread>
#include <optional>
#include <set>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <GlobalLogger.hpp>


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VkContext {
    public:
        VkContext(GLFWwindow* window);
        ~VkContext();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
    public:
        GLFWwindow* _window{nullptr};
        VkInstance _instance{};
        VkSurfaceKHR _surface{};
        VkDebugUtilsMessengerEXT _debugMessenger{};
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _device{};
        VkQueue _graphicsQueue{};
        VkQueue _presentQueue{};
        VkSwapchainKHR _swapChain{};
        VkFormat _swapChainImageFormat{};
        VkExtent2D _swapChainImageExtent{};
        std::vector<VkImage> _swapChainImages{};
        std::vector<VkImageView> _swapChainImageViews{};

        inline static std::string name{"learn"};

        static std::vector<const char*> getRequiredExtensions();

        void setupDebugMessenger();

        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();

        bool isDeviceSuitable(const VkPhysicalDevice& device) const;
        static bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);
        static bool checkValidationLayerSupport();

        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device) const;
        SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device) const;

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
};