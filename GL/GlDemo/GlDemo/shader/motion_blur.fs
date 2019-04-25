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
    
    Color += texture(screenTexture, TexCoord) * 0.4;
    TexCoord -= MotionVector;
    Color += texture(screenTexture, TexCoord) * 0.3;
    TexCoord -= MotionVector;
    Color += texture(screenTexture, TexCoord) * 0.2;
    TexCoord -= MotionVector;
    Color += texture(screenTexture, TexCoord) * 0.1;

    FragColor = Color;
	//FragColor = vec4(texture(screenTexture, TexCoords).rgba);
}