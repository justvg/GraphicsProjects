#version 330 core

out vec4 FragColor;

in vec2 TextureCoord;
in vec3 Normal;
in vec3 FragmentPosWorld;

uniform sampler2D Texture;
uniform vec3 LightColor; 
uniform vec3 LightPosWorld;
uniform vec3 CamPosWorld;

float Attenuation(vec3 A, vec3 B, float RStart, float REnd)
{
	float Len = length(A - B);
	float Result = (REnd - Len) / (REnd - RStart);
	Result = clamp(Result, 0.0, 1.0);
	return (Result);
}

void main()
{
	float AmbientStrength = 0.1;
	vec3 Ambient = AmbientStrength * LightColor;

	vec3 LightDir = normalize(LightPosWorld - FragmentPosWorld);
	float Atten = Attenuation(LightPosWorld, FragmentPosWorld, 0.0, 10.0); 
	vec3 Diffuse = Atten * LightColor * max(dot(Normal, LightDir), 0.0);

	vec3 ViewDir = normalize(CamPosWorld - FragmentPosWorld);
	vec3 ReflectDir = reflect(-LightDir, Normal);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), 32);
	vec3 Specular = Atten * Spec * LightColor;

	vec3 Result = (Ambient + Diffuse + Specular) * texture(Texture, TextureCoord).xyz;

	FragColor = vec4(Result, 1.0);
}