#pragma once
#include <glm/glm.hpp>

class Drawable {
    public:
        virtual ~Drawable() = default;
        virtual void init() {};
        virtual void update() {};
        virtual void render(double delta, const glm::mat4& cameraMatrix) {};
        virtual void resize(int width, int height) {};
};
