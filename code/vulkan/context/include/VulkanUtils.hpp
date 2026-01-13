#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#ifdef __WIN32
#include <vulkan/vulkan_win32.h>
#include <GLFW/glfw3native.h>
#endif


inline VkResult createGlfwSurface(const VkInstance& instance, VkSurfaceKHR& surface, GLFWwindow* window) {
    return glfwCreateWindowSurface(instance, window, nullptr, &surface);
}

inline VkResult createWin32Surface(const VkInstance &instance, VkSurfaceKHR &surface, GLFWwindow *window) {
    VkWin32SurfaceCreateInfoKHR win32KHRInfo{};
    win32KHRInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32KHRInfo.hwnd = glfwGetWin32Window(window);
    win32KHRInfo.hinstance = GetModuleHandle(nullptr);

    return vkCreateWin32SurfaceKHR(instance, &win32KHRInfo, nullptr, &surface);
}

inline VkResult createBasicGraphicsDevice(PhysicalDeviceContext& physicalDeviceContext, VkDevice& device, VkSurfaceKHR& surface) {
    VkDeviceCreateInfo deviceInfo{};
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    auto& graphicsQueueFamily = physicalDeviceContext.queryQueueFamily(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT, 2);
    if (graphicsQueueFamily.second.queueFlags == VK_QUEUE_FLAG_BITS_MAX_ENUM) return VK_NOT_READY;
    auto& presentQueueFamily = physicalDeviceContext.queryPresentQueueFamily(surface);
    if (presentQueueFamily.second.queueFlags == VK_QUEUE_FLAG_BITS_MAX_ENUM) return VK_NOT_READY;

    std::set queueFamilies = {graphicsQueueFamily.first, presentQueueFamily.first};

    graphicsQueueFamily.second.occupies(queueFamilies.size());

    float priority = 1.0f;
    for (uint32_t index : queueFamilies) {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = index;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &priority;
        queueInfos.push_back(queueInfo);
    }
    const char* extensionNames[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkPhysicalDeviceFeatures features{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = queueInfos.size();
    deviceInfo.pQueueCreateInfos = queueInfos.data();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = extensionNames;
    deviceInfo.pEnabledFeatures = &features;

    return vkCreateDevice(physicalDeviceContext, &deviceInfo, nullptr, &device);
}

inline VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            glog.log<DefaultLevel::Debug>("验证层: " + std::string(pCallbackData->pMessage));
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            glog.log<DefaultLevel::Info>("验证层: " + std::string(pCallbackData->pMessage));
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            glog.log<DefaultLevel::Warn>("验证层: " + std::string(pCallbackData->pMessage));
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            glog.log<DefaultLevel::Error>("验证层: " + std::string(pCallbackData->pMessage));
            break;
        }
        default:;
    }
    return VK_FALSE;
}

