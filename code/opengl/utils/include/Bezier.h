#pragma once
#include <glm/glm.hpp>

/**
 * @brief 三阶贝塞尔曲线封装
 */
class Bezier {
    public:
        /**
         * @brief 三阶贝塞尔曲线构造
         * @details 他似乎不需要详细注释[划掉]
         * @param start 起点
         * @param control0 控制点 0
         * @param control1 控制点 1
         * @param end 终点
         */
        Bezier(const glm::vec2& start, const glm::vec2& control0, const glm::vec2& control1, const glm::vec2& end);

        /**
         * @brief 起点重点为对角线[0 - 1]曲线构造
         * @details 他似乎不需要详细注释[划掉]
         * @param control0 控制点 0
         * @param control1 控制点 1
         */
        Bezier(const glm::vec2& control0, const glm::vec2& control1);
        ~Bezier() = default;

        Bezier(const Bezier& other) = default;
        Bezier& operator = (const Bezier& other) = default;
        Bezier(Bezier&& other) noexcept = default;
        Bezier& operator = (Bezier&& other) noexcept = default;

        /**
         * @brief 计算贝塞尔曲线函数[x]
         * @details 他似乎不需要详细注释[划掉]
         * @param t 自变量
         * @return 函数值
         */
        [[nodiscard]] double x(double t) const;

        /**
         * @brief 计算贝塞尔曲线函数[y]
         * @details 他似乎不需要详细注释[划掉]
         * @param t 自变量
         * @return 函数值
         */
        [[nodiscard]] double y(double t) const;

        /**
         * @brief 贝塞尔曲线函数求导[x]
         * @details 他似乎不需要详细注释[划掉]
         * @param t 自变量
         * @return 函数值
         */
        [[nodiscard]] double derivative_x(double t) const;

        /**
         * @brief 贝塞尔曲线函数求导[y]
         * @details 他似乎不需要详细注释[划掉]
         * @param t 自变量
         * @return 函数值
         */
        [[nodiscard]] double derivative_y(double t) const;

        /**
         * @brief 逆贝塞尔曲线函数[x]
         * @details 他似乎不需要详细注释[划掉]
         * @param t 函数值
         * @param accuracy 精度
         * @return 自变量
         */
        [[nodiscard]] double inverse_x(double t, size_t accuracy = 3) const;

        /**
         * @brief 逆贝塞尔曲线函数[y]
         * @details 他似乎不需要详细注释[划掉]
         * @param t 函数值
         * @param accuracy 精度
         * @return 自变量
         */
        [[nodiscard]] double inverse_y(double t, size_t accuracy = 3) const;

        [[nodiscard]] glm::vec2 get(double t) const;

        glm::vec2 operator [] (double t) const;
    private:
        glm::vec2 _start{};
        glm::vec2 _control0{};
        glm::vec2 _control1{};
        glm::vec2 _end{};
};