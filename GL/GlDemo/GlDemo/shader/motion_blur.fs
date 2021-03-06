#version 330

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D motionTexture;
    
out vec4 FragColor;
                                                                                        
void main()
{                                    
    vec2 MotionVector = texture(motionTexture, TexCoords).xy / 2.0;

    vec4 Color = vec4(0.0);

    vec2 TexCoord = TexCoords;
    
    Color += texture(screenTexture, TexCoord).rgba * 0.4;
    TexCoord += MotionVector;
    Color += texture(screenTexture, TexCoord).rgba * 0.3;
    TexCoord += MotionVector;
    Color += texture(screenTexture, TexCoord).rgba * 0.2;
    TexCoord += MotionVector;
    Color += texture(screenTexture, TexCoord).rgba * 0.1;

    FragColor = Color;
	vec2 col = texture(screenTexture, TexCoord).xy;
	vec3 color = texture(screenTexture, TexCoord).rgb;
	FragColor = vec4(color, 1.0);
	//FragColor = vec4(texture(screenTexture, TexCoords).rgba);
}