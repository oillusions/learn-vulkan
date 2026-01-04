#pragma once
#include <map>

#include "Shader.h"
#include "ShaderUniform.h"

/**
 * @brief 着色器程序包装
 */
class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        /**
         * @brief 附加着色器
         * @details 他似乎不需要详细注释[划掉]
         * @param shader 着色器
         */
        void attach(const Shader& shader) const;

        /**
         * @brief 链接着色器
         * @details 他似乎不需要详细注释[划掉]
         * @return 似乎不需要注释[划掉]
         */
        [[nodiscard]] bool link();

        /**
         * @brief 使用着色器
         * @details 他似乎不需要详细注释[划掉]
         */
        void use() const;

        /**
         * @brief 通过类型转换操作符获取着色器程序id
         * @details 他似乎不需要详细注释[划掉]
         * @return 着色器程序id
         */
        operator unsigned int() const {
            return _location;
        }

        /**
         * @brief 通过下标操作符获取着色器变量引用
         * @details 他似乎不需要详细注释[划掉]
         * @param identifier 着色器标识符
         * @return 着色器变量引用
         */
        ShaderUniform& operator[] (const std::string& identifier) {
            return uniformMap.at(identifier);
        }

        ShaderUniform& operator[] (const char* name) {
            return uniformMap.at(std::string(name));
        }

    private:
        unsigned int _location;
        bool isLink {};
        std::map<std::string, ShaderUniform> uniformMap;

        void initUniformMap();
        void deInitUniformMap();

};
