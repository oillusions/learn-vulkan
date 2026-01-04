#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

uniform mat4 Transform;

void main() {
    gl_Position = Transform * vec4(aPos.xyz, 1.0f);
    TexCoord = aTexCoord;
}