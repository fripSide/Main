#version 450 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}