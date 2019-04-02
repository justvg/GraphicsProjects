#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT
{
	vec3 FragPosWorld;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct light
{
    vec3 PosWorld;
    vec3 Color;
};

#define LIGHTS_COUNT 4
uniform light Lights[LIGHTS_COUNT]; 
uniform sampler2D DiffuseTexture;

void main()
{	
    vec3 Color = texture(DiffuseTexture, fs_in.TexCoords).rgb;
    vec3 Ambient = 0.0 * Color;
    vec3 Lighting = vec3(0.0);
    for(int I = 0; I < LIGHTS_COUNT; I++)
    {
        vec3 LightDir = normalize(Lights[I].PosWorld - fs_in.FragPosWorld);
        float Diff = max(dot(LightDir, fs_in.Normal), 0.0);
        vec3 Diffuse = Lights[I].Color * Diff * Color;
        float Distance = length(Lights[I].PosWorld - fs_in.FragPosWorld);
        Diffuse *= 1.0 / (Distance * Distance);
        Lighting += Diffuse;
    }

    FragColor = vec4(Ambient + Lighting, 1.0);

    float Brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(Brightness > 1.0)
    {
        BrightColor = FragColor;
    }
}

