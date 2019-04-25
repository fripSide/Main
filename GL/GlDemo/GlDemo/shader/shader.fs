#version 330 core
in vec3 PColor;

out vec4 FragColor;

void main()
{
     FragColor = vec4(PColor, 1.0f);
} 