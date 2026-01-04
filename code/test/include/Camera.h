#pragma once
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

#include <EventBus.hpp>
#include <EventTypes.hpp>

#include "Interfaces.hpp"

class Camera: public Drawable {
    public:
        double _delta{};
        glm::vec3 _position{0.0, 0.0, 0.0};
        glm::quat _perspective{1.0, 0.0, 0.0, 0.0};
        glm::mat4 proj{1.0};
        double _center_x{}, _center_y{};
        bool _bindCursor{true};

        Camera() = default;
        ~Camera() override = default;

        [[nodiscard]] glm::mat4 viewMatrix() const;

        void init(GLFWwindow *window, const EventBus &ebus, int width, int height);
        void render(double delta, const glm::mat4 &cameraMatrix) override {};

        void reset();

        void handleCameraMove(const event::types::Keyboard_Event& content);
        void handleFrameCenterUpdate(const event::types::FrameSize_Event& content);
        void handleCameraPerspective(const event::types::MouseMove_Event& content);
};
