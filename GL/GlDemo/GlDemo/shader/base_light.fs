#version 330 core
in vec3 PColor;

out vec4 FragColor;
in vec2 uv0;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
     FragColor = vec4(PColor, 1.0);
	FragColor = vec4(uv0.x, uv0.y, uv0.x, 1.0);
	FragColor = mix(texture(texture1, uv0), texture(texture2, uv0), 0.2);
	
} 