#version 330 core
layout (location = 0) in vec3 aPos;

vec3 color;
uniform mat4 mvp;

out vec3 PColor;

void main()
{
	color = vec3(1.0, 0.5, 1.0);
    gl_Position = mvp * vec4(aPos, 1.0);
	PColor = color;
}