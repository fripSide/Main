#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

uniform vec4 coral = vec4(1.0f, 0.5f, 0.31f, 1.f);

void main() {
    FragColor = texture(texture_diffuse1, TexCoords) * coral;
}