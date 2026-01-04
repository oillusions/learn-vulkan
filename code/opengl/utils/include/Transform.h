#pragma once
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

class Transform {
    public:
        Transform() = default;
        ~Transform() = default;

        Transform& configInverse(bool _isInverse = true);

        Transform& origin(const glm::vec3& vec);
        Transform& translate(const glm::vec3& vec);
        Transform& scale(const glm::vec3& vec);
        Transform& rotate(const glm::vec3& vec);
        Transform& rotate(const glm::quat& quat);

        Transform& setTranslate(const glm::vec3& vec);
        Transform& setScale(const glm::vec3& vec);
        Transform& setRotate(const glm::vec3& vec);
        Transform& setRotate(const glm::quat& quat);

        Transform& resetOrigin();
        Transform& resetTranslate();
        Transform& resetScale();
        Transform& resetRotate();

        const glm::vec3 &getOrigin() const;
        const glm::vec3 &getPosition() const;
        const glm::quat &getRotation() const;
        const glm::vec3 &getScale() const;

        static glm::mat4 worldMatrix(const std::vector<Transform>& transforms);
        static glm::mat4 worldMatrix(const std::vector<std::reference_wrapper<const Transform>> &transforms);
        static glm::mat4 worldMatrix(const std::vector<std::reference_wrapper<Transform>> &transforms);

        bool isDirty() const;

        [[nodiscard]] glm::mat4 getMatrix() const;

        operator glm::mat4() const{
            return getMatrix();
        }
    private:
        bool _isInverse{false};
        glm::vec3 _position{0.0};
        glm::quat _rotation{1.0, 0.0, 0.0, 0.0};
        glm::vec3 _scale{1.0, 1.0, 1.0};
        glm::vec3 _origin{0.0};
        mutable glm::mat4 _cacheMatrix{1.0f};
        mutable bool _isDirty{true};

        static inline glm::quat vec3toQuat(const glm::vec3& vec);
};
