#version 330 core
in vec3 PColor;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec2 MotionVector;
 
in vec2 uv0;
in vec4 ClipSpacePos;
in vec4 PrevClipSpacePos; 

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = vec4(PColor, 1.0);
	//FragColor = vec4(uv0.x, uv0.y, uv0.x, 1.0);
	FragColor = mix(texture(texture1, uv0), texture(texture2, uv0), 0.2);
	
	vec3 NDCPos = (ClipSpacePos / ClipSpacePos.w).xyz;
	vec3 PrevNDCPos = (PrevClipSpacePos / PrevClipSpacePos.w).xyz;
	vec2 motionVector = (NDCPos - PrevNDCPos).xy;
	MotionVector = motionVector;
	//FragColor = vec4(motionVector.x, motionVector.y, 1.0, 1.0);
} 