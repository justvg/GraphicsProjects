#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragmentPosWorld;
out vec2 TexCoord;

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Model = mat4(1.0);

void main()
{
	Normal = normalize(mat3(Model) * aNormal);
	FragmentPosWorld = vec3(Model * vec4(aPosition, 1.0));
	TexCoord = aTexCoord;
	
	gl_Position = Projection *  View *  Model * vec4(aPosition,  1.0);
}