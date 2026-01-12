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
