#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;

out vec2 TextureCoord;
out vec3 Normal;
out vec3 FragmentPosWorld;

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Model = mat4(1.0);

void main()
{
	TextureCoord = aTextureCoord;
	// NOTE(georgy): Don't normalize &/or inverse transpose because the matrix doesn't contain scale
	Normal = mat3(Model) * aNormal;
	FragmentPosWorld = vec3(Model * vec4(aPosition, 1.0));

	gl_Position = Projection *  View *  Model * vec4(aPosition,  1.0);
}