#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


inline void createWin32Surface(const VkInstance &instance, VkSurfaceKHR &surface, GLFWwindow *window) {
    glfwInitVulkanLoader(vkGetInstanceProcAddr);
    glfwCreateWindowSurface(instance, window, nullptr, &surface);
}