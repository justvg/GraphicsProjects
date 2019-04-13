#version 330 core

out vec4 FragColor;

uniform sampler2D gPos;
uniform sampler2D gNormals;
uniform sampler2D gAlbedo;
uniform sampler2D SSAO;

in vec2 TexCoords;

struct light
{
	vec3 PosView;
	vec3 Color;
};

#define LIGHTS_COUNT 2
uniform light Lights[LIGHTS_COUNT];

void main()
{
	vec3 FragPos = texture(gPos, TexCoords).rgb;
	vec3 Normal = texture(gNormals, TexCoords).rgb;
	vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
	float AmbientOcclusion = texture(SSAO, TexCoords).r;

	vec3 Ambient = 0.3 * Albedo * AmbientOcclusion;

	vec3 Lighting = Ambient;
	for(int I = 0; I < LIGHTS_COUNT; I++)
	{
		float Distance = length(Lights[I].PosView - FragPos);
		float Attenuation = 1.0 / (Distance * Distance);

		vec3 LightDir = normalize(Lights[I].PosView - FragPos);
		float Diff = max(dot(LightDir, Normal), 0.0);
		vec3 Diffuse = Albedo * Diff * Lights[I].Color;
		Diffuse *= Attenuation;
		Lighting += Diffuse;
	}

	Lighting = sqrt(Lighting);
	FragColor = vec4(Lighting, 1.0);
}