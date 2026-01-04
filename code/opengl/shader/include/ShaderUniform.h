#pragma once
#include <string>
#include <glm/glm.hpp>

/**
 * @brief 着色器变量包装
 */
class ShaderUniform {
    public:
        ShaderUniform(): _location(-1), _type(0) {};

        /**
         * @brief 着色器变量构造
         * @details 他似乎不需要详细注释[划掉]
         * @param id 着色器变量id
         * @param type 着色器变量类型
         */
        ShaderUniform(int id, unsigned int type);

        ~ShaderUniform() = default;

        /**
         * @brief 通过类型转换操作符获取着色器变量id
         * @details 他似乎不需要详细注释[划掉]
         * @return 着色器变量id
         */
        operator int() const {
            return _location;
        }

        void setFloat(float value);
        void setInt(int value);
        void setBool(bool value);
        void setF4(const glm::vec4& value);
        void setMat4(const glm::mat4& value) const;
    private:
        const int _location;
        const unsigned int _type;

};
