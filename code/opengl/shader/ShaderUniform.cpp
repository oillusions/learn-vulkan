#include "ShaderUniform.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

ShaderUniform::ShaderUniform(const int id, const unsigned int type): _location(id), _type(type) {}

void ShaderUniform::setFloat(float value) {
    if (_type == GL_FLOAT) {
        glUniform1f(_location, value);
    }
}

void ShaderUniform::setInt(int value) {
    if (_type == GL_INT) {
        glUniform1i(_location, value);
    }
}

void ShaderUniform::setBool(bool value) {
    if (_type == GL_BOOL) {
        glUniform1i(_location, value);
    }
}

void ShaderUniform::setF4(const glm::vec4& f4) {
    if (_type == GL_FLOAT_VEC4) {
        glUniform4f(_location, f4.x, f4.y, f4.z, f4.w);
    }
}

void ShaderUniform::setMat4(const glm::mat4& value) const {
    if (_type == GL_FLOAT_MAT4) {
        glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }
}




