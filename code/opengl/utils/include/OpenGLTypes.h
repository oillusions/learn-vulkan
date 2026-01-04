#pragma once

#include <glad/glad.h>

#include <RAIIWrapper.hpp>
#include <VectorStruct.hpp>
#include <OpenGLTypes.h>

#include "VertexLayout.hpp"

class Texture: public RAIIWrapper<GLuint> {
    public:
        explicit Texture(const GLuint texObjectID = 0) {
            _value = texObjectID;
        }
        ~Texture() override {
            glDeleteTextures(1, &_value);
        }
};

/**
 * @brief 顶点数组对象的RAII包装
 * @details 他似乎不需要详细注释[划掉]
 */
class VertexArrays: public RAIIWrapper<GLuint> {
public:
    explicit VertexArrays(const GLuint value = 0) {
        _value = value;
    }
    ~VertexArrays() override {
        glDeleteVertexArrays(1, &_value);
    }
};

/**
 * @brief 缓冲区对象的RAII包装
 * @details 他似乎不需要详细注释[划掉]
 */
class BufferObject: public RAIIWrapper<GLuint> {
public:
    explicit BufferObject(const GLuint value = 0) {
        _value = value;
    }

    ~BufferObject() override {
        glDeleteBuffers(1, &_value);
    }
};

class ModelVertexMeta: public RAIIWrapper<VectorStruct<BufferObject, BufferObject>> {
    public:
        ModelVertexMeta(const VertexLayout<float>& vertices) {
            if (!vertices.rawIndices().empty()) {

            }
            auto& va = vertices.ExpandIndices();
            auto& ea = vertices.bufferOfIndices();
        }
        ~ModelVertexMeta() override {
            auto& [a, b] = _value.memberPack;
            glDeleteBuffers(1, &a);
            glDeleteBuffers(1, &b);
        }
    VectorStruct<BufferObject, BufferObject> aaa;
};

class FrameBuffer: public RAIIWrapper<GLuint> {
    public:
        explicit FrameBuffer(const GLuint value = 0) {
            _value = value;
        }
        ~FrameBuffer() override {
            glDeleteFramebuffers(1, &_value);
        }
};

class RenderBuffer: public RAIIWrapper<GLuint> {
    public:
        explicit RenderBuffer(const GLuint value = 0) {
            _value = value;
        }
        ~RenderBuffer() override {
            glDeleteRenderbuffers(1, &_value);
        }
};

