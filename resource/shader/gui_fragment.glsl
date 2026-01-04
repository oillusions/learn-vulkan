#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D _texture;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}