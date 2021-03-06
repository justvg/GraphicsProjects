#version 330 core

out vec4 FragColor;

in vec2 TextureCoord;
in vec3 Normal;
in vec3 FragmentPosWorld;

uniform vec3 CamPosWorld;

struct material
{
	sampler2D Diffuse;
	sampler2D Specular;

	float Shininess;
};

uniform material Material; 

struct dir_light
{
	vec3 Direction;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform dir_light DirLight;

vec3 CalcDirLight(dir_light Light, vec3 Normal, vec3 ViewDir)
{
	Light.Direction = normalize(Light.Direction);

	float DiffComponent = max(dot(Light.Direction, Normal), 0.0);

	vec3 ReflectDir = reflect(-Light.Direction, Normal);
	float SpecComponent = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Ambient = Light.Ambient * texture(Material.Diffuse, TextureCoord).xyz;	
	vec3 Diffuse = Light.Diffuse * DiffComponent * texture(Material.Diffuse, TextureCoord).xyz;
	vec3 Specular = Light.Specular * SpecComponent * texture(Material.Specular, TextureCoord).xyz;

	vec3 Result = Ambient + Diffuse + Specular;
	return (Result);
}

float Attenuation(vec3 A, vec3 B, float RStart, float REnd)
{
	float Len = length(A - B);
	float Result = (REnd - Len) / (REnd - RStart);
	Result = clamp(Result, 0.0, 1.0);
	return (Result);
}

struct point_light
{
	vec3 Position;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

#define POINT_LIGHTS 1
uniform point_light PointLights[POINT_LIGHTS];

vec3 CalcPointLight(point_light Light, vec3 Normal, vec3 ViewDir, vec3 FragmentPos)
{
	float Atten = Attenuation(Light.Position, FragmentPos, 0.0, 5.0);

	vec3 LightDir = normalize(Light.Position - FragmentPos);
	float DiffComponent = max(dot(LightDir, Normal), 0.0);

	vec3 ReflectDir = reflect(-LightDir, Normal);
	float SpecComponent = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);

	vec3 Ambient = Light.Ambient * Atten * texture(Material.Diffuse, TextureCoord).xyz;
	vec3 Diffuse = Light.Diffuse * Atten * DiffComponent * texture(Material.Diffuse, TextureCoord).xyz;
	vec3 Specular = Light.Specular * Atten * SpecComponent * texture(Material.Specular, TextureCoord).xyz;

	vec3 Result = Ambient + Diffuse + Specular;
	return (Result);
}

struct spot_light
{
	vec3 Position;
	vec3 Direction;

	float Penumbra;
	float Umbra;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform spot_light SpotLight;

vec3 CalcSpotlight(spot_light Light, vec3 Normal, vec3 ViewDir, vec3 FragmentPos)
{
	float Atten = Attenuation(Light.Position, FragmentPos, 0.0, 5.0);

	Light.Direction = normalize(Light.Direction);
	vec3 LightDir = normalize(Light.Position - FragmentPos);
	float Theta = dot(-Light.Direction, LightDir);
	float Intensity = clamp((Theta - Light.Umbra) / (Light.Penumbra - Light.Umbra), 0.0, 1.0);

	float DiffComponent = max(dot(LightDir, Normal), 0.0);
	
	vec3 ReflectDir = reflect(-LightDir, Normal);
	float SpecComponent = pow(max(dot(ReflectDir, ViewDir), 0.0), Material.Shininess);
	
	vec3 Ambient = Light.Ambient * Atten * Intensity * texture(Material.Diffuse, TextureCoord).xyz;
	vec3 Diffuse = Light.Diffuse * Atten * Intensity * texture(Material.Diffuse, TextureCoord).xyz;
	vec3 Specular = Light.Specular * Atten * Intensity * texture(Material.Specular, TextureCoord).xyz;

	vec3 Result = Ambient + Diffuse + Specular;
	return (Result);
}

void main()
{	
	vec3 ViewDir = normalize(CamPosWorld - FragmentPosWorld);
	vec3 Result = CalcDirLight(DirLight, Normal, ViewDir);

	for(int PointLightIndex = 0; PointLightIndex < POINT_LIGHTS; PointLightIndex++)
	{
		Result += CalcPointLight(PointLights[PointLightIndex], Normal, ViewDir, FragmentPosWorld);
	}

	Result += CalcSpotlight(SpotLight, Normal, ViewDir, FragmentPosWorld);

	FragColor = vec4(Result, 1.0);
}