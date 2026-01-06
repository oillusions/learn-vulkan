#pragma once
#include <string>


/**
 * @brief 着色器包装
 */
class Shader {
    public:
        enum ShaderType {
            Vertex = GL_VERTEX_SHADER,
            Geometry = GL_GEOMETRY_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
        };

        /**
         * @brief 着色器构造并编译
         * @details 他似乎不需要详细注释[划掉]
         * @param shaderType 着色器类型
         * @param shaderSource 着色器源码
         */
        explicit Shader(ShaderType shaderType, const std::string& shaderSource);
        ~Shader();

        /**
         * @brief 通过类型转换操作符着色器对象id
         * @details 他似乎不需要详细注释[划掉]
         * @return 着色器id
         */
        operator unsigned int() const {
            return _location;
        }
    private:
        unsigned int _location;
};
