#pragma once
#include <string>

#include <glad/glad.h>

#include <OpenGLTypes.h>
#include <Interfaces.hpp>

#include <LayerScene.hpp>
#include <Transform.h>


class Layer: public Drawable {
    public:
        int32_t layerDepth;

        Layer(const std::string& identifier, int32_t depth, const int width, const int height):
            layerDepth(depth), identifier(identifier), layerWidth(width), layerHeight(height) {
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glGenTextures(1, &fat);
            glBindTexture(GL_TEXTURE_2D, fat);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);

            reFrameSize(layerWidth, layerHeight);
        }

        void init() override {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            for (auto& e : sceneTree.list()) {
                e.getCarried().init();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void update() override {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            for (auto& e : sceneTree.list()) {
                e.getCarried().update();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void render(double delta, const glm::mat4 &cameraMatrix) override {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            for (auto& e : sceneTree.list()) {
                e.getCarried().render(delta, cameraMatrix);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void resize(const int width, const int height) override {
            if (width <= 0 || height <= 0) {
                return;
            }
            reFrameSize(width, height);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            for (auto& e : sceneTree.list()) {
                e.getCarried().resize(width, height);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        LayerSceneElement& layerScene() {
            return sceneTree;
        }

    protected:
        Texture fat;
        RenderBuffer rbo;
        FrameBuffer fbo;
        std::string identifier;
        LayerSceneElement sceneTree{"root"};
        int layerWidth{}, layerHeight{};

        void reFrameSize(int width, int height) {
            layerWidth = width;
            layerHeight = height;

            glBindTexture(GL_TEXTURE_2D, fat);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, layerWidth, layerHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, layerWidth, layerHeight);

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                glog.log<DefaultLevel::Warn>("帧缓冲不完整, 正在附加附件");
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fat, 0);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
            }
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                glog.log<DefaultLevel::Error>("帧缓冲依旧不完整, 强制终止");
                std::terminate();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
};
