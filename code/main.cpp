#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_VULKAN

#ifdef __WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <iostream>
#include <fstream>
#include <thread>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <GLFW/glfw3.h>


#include <GlobalLogger.hpp>
#include <ResourceTypes.hpp>
#include <EventTypes.hpp>
#include <EvkContext.h>
#include "VulkanUtils.hpp"


using namespace std;
namespace fs = filesystem;

double deltaTime{};
double lastTime{};

constexpr int width = 800, height = 600;
string name = "learn";

GLFWwindow* window{nullptr};
void render() {
    glfwSwapInterval(1);

    VulkanContext context{};

    auto surface = context.createSurface(createGlfwSurface, window);


    double currentTime{};
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }
}

int main() {
    globalLogger::_minLevel = DefaultLevel::Debug;
    glog.log(DefaultLevel::Info, "程序已启动");

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        glog.log(DefaultLevel::Error, "窗口创建失败");
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, event::func::key_callback);
    glfwSetScrollCallback(window, event::func::scroll_callback);
    glfwSetCursorPosCallback(window, event::func::mouse_callback);
    glfwSetMouseButtonCallback(window, event::func::mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, event::func::frameBuffer_size_callback);

    thread renderThread(render);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    renderThread.join();
    glfwTerminate();
    glfwDestroyWindow(window);

    glog.log<DefaultLevel::Info>("程序已结束");
    return 0;
}