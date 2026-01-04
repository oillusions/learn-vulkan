#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <windows.h>
#include <thread>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <GlobalLogger.hpp>
#include <ResourceTypes.hpp>


#include <Camera.h>
#include <EventTypes.hpp>


using namespace std;
namespace fs = filesystem;

double deltaTime{};
double lastTime{};

int width = 800, height = 600;
string name = "learn";

GLFWwindow* window{nullptr};

// template<typename T, typename... Args>
// T cast(Args&&... args) {
//     return {forward<Args>(args)...};
// }

void render() {
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    // unique_ptr<Camera> camera = make_unique<Camera>();
    // camera->init(window, gEbus, width, height);

    double currentTime{};
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glfwSwapBuffers(window);
    }
}

int main() {

    globalLogger::_minLevel = DefaultLevel::Debug;
    glog.log(DefaultLevel::Info, "程序已启动");


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    thread openglThread(render);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    openglThread.join();
    glfwTerminate();

    glog.log<DefaultLevel::Info>("程序已结束");
    return 0;
}