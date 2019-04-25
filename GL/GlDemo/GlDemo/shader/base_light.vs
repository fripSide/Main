#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 in_uv;

vec3 color;
uniform mat4 mvp;

out vec2 uv0;
out vec3 PColor;

void main()
{
	color = vec3(1.0, 0.5, 1.0);
    gl_Position = mvp * vec4(aPos, 1.0);
	uv0 = in_uv;
	PColor = color;
}