#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <windows.h>
#include <thread>

#include <GlobalLogger.hpp>
#include <ResourceTypes.hpp>
#include <EventTypes.hpp>

#include <Camera.h>

#include <VkContext.h>
#include <TestRender.h>


using namespace std;
namespace fs = filesystem;

double deltaTime{};
double lastTime{};

constexpr int width = 800, height = 600;
string name = "learn";

GLFWwindow* window{nullptr};

void render() {
    glfwSwapInterval(1);

    VkContext context(window);

    TestRender test(context);


    // unique_ptr<Camera> camera = make_unique<Camera>();wa
    // camera->init(window, gEbus, width, height);

    double currentTime{};
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        test.render();
    }
    vkDeviceWaitIdle(context._device);
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