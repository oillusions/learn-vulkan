#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


inline VkResult createWin32Surface(const VkInstance &instance, VkSurfaceKHR &surface, GLFWwindow *window) {
    VkWin32SurfaceCreateInfoKHR win32KHRInfo{};
    win32KHRInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32KHRInfo.hwnd = glfwGetWin32Window(window);
    win32KHRInfo.hinstance = GetModuleHandle(nullptr);

    return vkCreateWin32SurfaceKHR(instance, &win32KHRInfo, nullptr, &surface);
}
