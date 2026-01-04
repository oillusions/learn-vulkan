#include "ShaderProgram.h"
#include <glad/glad.h>
#include <iostream>

using namespace std;

ShaderProgram::ShaderProgram() {
    _location = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(_location);
    isLink = false;
}

void ShaderProgram::attach(const Shader& shader) const {
    glAttachShader(_location, shader);
}

bool ShaderProgram::link() {
    int success;
    char infoLog[512];
    glLinkProgram(_location);
    glGetProgramiv(_location, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_location, 512, nullptr, infoLog);
        cerr << "着色器程序链接错误: " << infoLog << endl;
        return isLink = false;
    }
    isLink = true;
    initUniformMap();
    return isLink;
}

void ShaderProgram::use() const {
    glUseProgram(_location);
}

void ShaderProgram::initUniformMap() {
    int count{};
    int maxLen{};
    glGetProgramiv(_location, GL_ACTIVE_UNIFORMS, &count);
    glGetProgramiv(_location, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);

    string nameStr;
    nameStr.resize(maxLen);
    for (unsigned int i = 0; i < count; ++i) {
        string uniformName;
        int location;
        int nameLen;
        int size;
        unsigned int type;

        glGetActiveUniform(_location, i, maxLen, &nameLen, &size, &type, const_cast<char*>(nameStr.c_str()));
        uniformName = nameStr.substr(0, nameLen);

        location = glGetUniformLocation(_location, uniformName.c_str());

        uniformMap.emplace(piecewise_construct,
            forward_as_tuple(uniformName),
            forward_as_tuple(location, type)
        );
        nameStr.erase();
        nameStr.resize(maxLen);
    }
}

void ShaderProgram::deInitUniformMap() {
    uniformMap.clear();
}
