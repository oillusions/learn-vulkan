#include "Shader.h"
#include <glad/glad.h>
#include <iostream>

using namespace std;

Shader::Shader(ShaderType shaderType, const string &shaderSource) {
    if (shaderSource.empty()) {
        cerr << "错误: 着色器源码为空" << endl;
        _location = 0;
        return;
    }
    _location = glCreateShader(static_cast<GLenum>(shaderType));

    int success;
    char infoLog[512];

    const GLchar* source = shaderSource.c_str();
    glShaderSource(_location, 1, &source, nullptr);
    glCompileShader(_location);
    glGetShaderiv(_location, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_location, 512, nullptr, infoLog);
        cerr << "着色器编译错误: " << infoLog << endl;
    }
}

Shader::~Shader() {
    glDeleteShader(_location);
}
