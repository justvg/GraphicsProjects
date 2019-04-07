#version 330 core

out vec4 FragColor;

uniform sampler2D gPos;
uniform sampler2D gNormals;
uniform sampler2D gAlbedo;

uniform vec2 ScreenSize;

struct light
{
	vec3 PosWorld;
	vec3 Color;

	float Linear;
	float Quadratic;
};

uniform light Light;

vec2 CalcTexCoords()
{
	vec2 Result = gl_FragCoord.xy / ScreenSize;

	return(Result);
}

void main()
{
	vec2 TexCoords = CalcTexCoords();
	vec3 FragPos = texture(gPos, TexCoords).rgb;
	vec3 Normal = texture(gNormals, TexCoords).rgb;
	vec3 Albedo = texture(gAlbedo, TexCoords).rgb;

	float Distance = length(Light.PosWorld - FragPos);
	float Attenuation = 1.0 / (1.0 + Light.Linear * Distance + Light.Quadratic * Distance * Distance);
	// float Attenuation = 1.0 / (Distance * Distance);

	vec3 Ambient = Albedo * 0.0;
	vec3 Lighting = vec3(0.0);
	vec3 LightDir = normalize(Light.PosWorld - FragPos);
	float Diff = max(dot(LightDir, Normal), 0.0);
	vec3 Diffuse = Albedo * Diff * Light.Color;
	Diffuse *= Attenuation;
	Lighting += Diffuse + Ambient;

	float Gamma = 2.0;
	Lighting = pow(Lighting, vec3(1.0 / Gamma));
	FragColor = vec4(Lighting, 1.0);
}