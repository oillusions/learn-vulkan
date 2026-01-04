#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include "GlobalLogger.hpp"

using namespace event::types;

void Camera::init(GLFWwindow *window, const EventBus &ebus, int width, int height) {
    ebus.subscribe<FrameSize_Event>("frame-size-callback", [this](const FrameSize_Event& content) {
        handleFrameCenterUpdate(content);
    });
    ebus.subscribe<MouseMove_Event>("mouse-move-callback", [this](const MouseMove_Event& content) {
        handleCameraPerspective(content);
    });
    ebus.subscribe<Keyboard_Event>("keyboard-callback", [this](const Keyboard_Event& content) {
        handleCameraMove(content);
    });
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    proj = glm::perspective(glm::radians(90.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    _center_x = static_cast<float>(width) / 2;
    _center_y = static_cast<float>(height) / 2;
}

glm::mat4 Camera::viewMatrix() const {
    glm::mat4 matrix{1.0};
    matrix = glm::translate(matrix, _position);
    matrix *= glm::mat4_cast(_perspective);
    matrix = glm::inverse(matrix);
    return proj * matrix;
}

void Camera::reset() {
    _position = {0.0f, 0.0f, 0.0f};
    _perspective = {1.0f, 0.0f, 0.0f, 0.0f};
}

void Camera::handleCameraPerspective(const  MouseMove_Event&content) {
    if (_bindCursor) {
        double offset_x{}, offset_y{};
        glog.log<DefaultLevel::Debug>("x:" + std::to_string(content.x) + ", y:" + std::to_string(content.y));
        offset_x = _center_x - content.x;
        offset_y = _center_y - content.y;
        glfwSetCursorPos(content.window, _center_x, _center_y);
        _perspective *= glm::quat({glm::radians( 5.0 * offset_y * _delta), 0.0f, 0.0f});
        _perspective *= glm::quat({0.0f, glm::radians(5.0 * offset_x * _delta), 0.0f});

    }

}

void Camera::handleFrameCenterUpdate(const FrameSize_Event &content) {
    _center_x = static_cast<float>(content.width) / 2;
    _center_y = static_cast<float>(content.height) / 2;
    if (content.width <= 0 || content.height <= 0) return;
    proj = glm::perspective(glm::radians(90.0f), static_cast<float>(content.width) / static_cast<float>(content.height), 0.1f, 100.0f);
}

void Camera::handleCameraMove(const Keyboard_Event &content) {
    switch (content.key) {
        case GLFW_KEY_W: {
            _position += glm::vec4({0.0, 0.0, -2.5 * _delta, 2.5 * _delta}) * glm::mat4_cast(glm::inverse(_perspective));
            break;
        }
        case GLFW_KEY_S: {
            _position += glm::vec4({0.0, 0.0, 2.5 * _delta, 2.5 * _delta}) * glm::mat4_cast(glm::inverse(_perspective));
            break;
        }
        case GLFW_KEY_A: {
            _position += glm::vec4({-2.5 * _delta, 0.0, 0.0, 2.5 * _delta}) * glm::mat4_cast(glm::inverse(_perspective));
            break;
        }
        case GLFW_KEY_D: {
            _position += glm::vec4({2.5 * _delta, 0.0, 0.0, 2.5 * _delta}) * glm::mat4_cast(glm::inverse(_perspective));
            break;
        }
        case GLFW_KEY_LEFT_ALT: {
            if (content.action == GLFW_PRESS) {
                _bindCursor = false;
                glfwSetInputMode(content.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else if (content.action == GLFW_RELEASE) {
                _bindCursor = true;
                glfwSetInputMode(content.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                glfwSetCursorPos(content.window, _center_x, _center_y);
            }
        }
        default:;
    }

}
