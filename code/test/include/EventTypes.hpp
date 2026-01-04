#pragma once
#include <GLFW/glfw3.h>

#include <EventBus.hpp>

inline EventBus gEbus{};

namespace event {
    namespace types {
        struct FrameSize_Event {
            GLFWwindow* window{};
            int width{};
            int height{};
        };

    struct Keyboard_Event {
        GLFWwindow* window{};
        int key{};
        int scancode{};
        int action{};
        int mods{};
    };

    struct MouseMove_Event {
        GLFWwindow* window{};
        double x{};
        double y{};
    };

    struct MouseButton_Event {
        int button;
        int action;
        int mods;
    };

    struct MouseScroll_Event {
        double x_offset;
        double y_offset;
    };
    }
    namespace func {
        inline void frameBuffer_size_callback(GLFWwindow *window, int width, int height) {
            types::FrameSize_Event content{window, width, height};
            gEbus.publish("frame-size-callback", content);
        }

        inline void scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
            types::MouseScroll_Event content{x_offset, y_offset};
            gEbus.publish("mouse-scroll-callback", content);
        }

        inline void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
            types::Keyboard_Event content{window, key, scancode, action, mods};
            gEbus.publish("keyboard-callback", content);
        }

        inline void mouse_callback(GLFWwindow *window, double x, double y) {
            types::MouseMove_Event content{window, x, y};
            gEbus.publish("mouse-move-callback", content);
        }

        inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
            types::MouseButton_Event content{button, action, mods};
            gEbus.publish("mouse-button-callback", content);
        }
    }
}
