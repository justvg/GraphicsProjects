#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT
{
	vec3 FragPosView;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Model = mat4(1.0);

uniform float ReverseNormal;

void main()
{	
	vs_out.FragPosView = vec3(View * Model * vec4(aPos, 1.0));
	mat3 NormalMatrix = transpose(inverse(mat3(View * Model)));
	vs_out.Normal = normalize(NormalMatrix * aNormal);
	vs_out.TexCoords = aTexCoords;

	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}