#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D motionTexture;

const float offset = 1.0 / 300;  

vec3 kernel_effects() {
	vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right
    );

    float kernel_1[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);
	
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col;
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
	return col;
}

// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
vec4 gaussian_blur() {
	float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
	float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);
	vec4 outColor = vec4(0.0);
	outColor += texture(screenTexture, (TexCoords / 1024.0)) * weight[0];
	for (int i = 0; i < 3; i++) {
		outColor += texture(screenTexture, (TexCoords + vec2(0.0, offset[i])) / 1024.0) * weight[i];
		outColor += texture(screenTexture, (TexCoords - vec2(0.0, offset[i])) / 1024.0) * weight[i];
	}
	return outColor;
}

const int SAMPLE_COUNT = 6;
vec4 radial_blur() {
	float factor = 0.03;
	vec2 center = vec2(0.5, 0.5); // 0 ~ 1
	vec4 outColor = vec4(0.0);
	vec2 dir = TexCoords - center;
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		vec2 uv = TexCoords + factor * dir * i;
		outColor += texture(screenTexture, uv);
	}
	outColor /= SAMPLE_COUNT;
	return outColor;
}

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
	vec4 color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
    FragColor = vec4(col, 1.0);
	color = vec4(kernel_effects(), 1.0);
	//color = radial_blur();
	//FragColor = color;
} 