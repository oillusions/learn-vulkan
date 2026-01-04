#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 Transform::getMatrix() const {
    if (!_isDirty) {
        return _cacheMatrix;
    }
    _isDirty = false;
    _cacheMatrix = glm::mat4(1.0f);
    _cacheMatrix = glm::translate(_cacheMatrix, _position - _origin);
    _cacheMatrix *= glm::mat4_cast(_rotation);
    _cacheMatrix = glm::scale(_cacheMatrix, _scale);
    _cacheMatrix = glm::translate(_cacheMatrix, _origin);

    if (_isInverse) {
        _cacheMatrix = glm::inverse(_cacheMatrix);
    }
    return _cacheMatrix;
}

Transform &Transform::configInverse(bool isInverse) {
    if (_isInverse != isInverse) {
        _isInverse = isInverse;
        _isDirty = true;
    }
    return *this;
}

Transform& Transform::origin(const glm::vec3& vec) {
    _origin = vec;
    _isDirty = true;
    return *this;
}

Transform& Transform::translate(const glm::vec3& vec) {
    _position += vec;
    _isDirty = true;
    return *this;
}

Transform& Transform::scale(const glm::vec3& vec) {
    _scale *= vec;
    _isDirty = true;
    return *this;
}

Transform& Transform::rotate(const glm::vec3& vec) {
    return rotate(vec3toQuat(vec));
}

Transform &Transform::rotate(const glm::quat &quat) {
    _rotation *= quat;
    _isDirty = true;
    return *this;
}

const glm::vec3& Transform::getOrigin() const {
    return _origin;
}

const glm::vec3 &Transform::getPosition() const {
    return _position;
}

const glm::quat &Transform::getRotation() const {
    return _rotation;
}

const glm::vec3 &Transform::getScale() const {
    return _scale;
}

Transform &Transform::setRotate(const glm::vec3& vec) {
    _rotation = vec3toQuat(vec);
    _isDirty = true;
    return *this;
}

Transform &Transform::setRotate(const glm::quat &quat) {
    _rotation = quat;
    _isDirty = true;
    return *this;
}

Transform &Transform::setScale(const glm::vec3 &vec) {
    _scale = vec;
    _isDirty = true;
    return *this;
}

Transform &Transform::setTranslate(const glm::vec3 &vec) {
    _position = vec;
    _isDirty = true;
    return *this;
}

Transform &Transform::resetOrigin() {
    _origin = {0.0f, 0.0f, 0.0f};
    _isDirty = true;
    return *this;
}

Transform &Transform::resetTranslate() {
    _position = {0.0f, 0.0f, 0.0f};
    _isDirty = true;
    return *this;
}


Transform &Transform::resetRotate() {
    _rotation = {1.0f, 0.0f, 0.0f, 0.0f};
    _isDirty = true;
    return *this;
}


Transform &Transform::resetScale() {
    _scale = {1.0f, 1.0f, 1.0f};
    _isDirty = true;
    return *this;
}


bool Transform::isDirty() const {
    return _isDirty;
}

glm::mat4 Transform::worldMatrix(const std::vector<Transform> &transforms)  {
    glm::mat4 out{1};
    for (const auto& t : transforms) {
        out *= t.getMatrix();
    }
    return out;
}

glm::mat4 Transform::worldMatrix(const std::vector<std::reference_wrapper<const Transform>> &transforms)  {
    glm::mat4 out{1};
    for (const auto& t : transforms) {
        out *= t.get().getMatrix();
    }
    return out;
}

glm::mat4 Transform::worldMatrix(const std::vector<std::reference_wrapper<Transform>> &transforms)  {
    glm::mat4 out{1};
    for (const auto& t : transforms) {
        out *= t.get().getMatrix();
    }
    return out;
}

glm::quat Transform::vec3toQuat(const glm::vec3 &vec) {
    return glm::quat(vec);
}
