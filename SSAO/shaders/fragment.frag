#version 330 core

layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in VS_OUT
{
	vec3 FragPosView;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D DiffuseTexture;

void main()
{	
    gPos = fs_in.FragPosView;

    gNormal = normalize(fs_in.Normal);

    gAlbedo = texture(DiffuseTexture, fs_in.TexCoords).rgb;
    // gAlbedo = vec3(0.95);
}