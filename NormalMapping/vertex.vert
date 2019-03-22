#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT 
{
	vec3 TangentFragPos;
	vec3 TangentLightPos;
	vec3 TangentCamPos;
	vec2 TexCoord;
} vs_out;

uniform vec3 LightPosWorld;
uniform vec3 CamPosWorld;

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Model = mat4(1.0);

void main()
{
	vs_out.TexCoord = aTexCoord;

	vec3 Tangent = normalize(mat3(Model) * aTangent);
	vec3 Normal = normalize(mat3(Model) * aNormal);
	Tangent = normalize(Tangent - dot(Tangent, Normal)*Normal);
	vec3 Bitangent = normalize(cross(Tangent, Normal));
	mat3 TBN = transpose(mat3(Tangent, Bitangent, Normal));
	
	vs_out.TangentFragPos = TBN * vec3(Model * vec4(aPosition, 1.0));
	vs_out.TangentLightPos = TBN * LightPosWorld;
	vs_out.TangentCamPos = TBN * CamPosWorld;

	gl_Position = Projection *  View *  Model * vec4(aPosition,  1.0);
}