#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 LightColor;

void main()
{	
	FragColor = vec4(LightColor, 1.0);
	float Brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(Brightness > 1.0)
    {
        BrightColor = FragColor;
    }
}