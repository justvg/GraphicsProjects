#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 BoneWeights;

out vec2 TexCoords;

const int MAX_BONES = 100;
uniform mat4 Bones[MAX_BONES];

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Model = mat4(1.0);

void main()
{	
	TexCoords = aTexCoords;

	mat4 BoneTransform = Bones[BoneIDs[0]] * BoneWeights[0];
	BoneTransform += Bones[BoneIDs[1]] * BoneWeights[1];
	BoneTransform += Bones[BoneIDs[2]] * BoneWeights[2];
	BoneTransform += Bones[BoneIDs[3]] * BoneWeights[3];

	vec4 Pos = BoneTransform * vec4(aPos, 1.0);
	gl_Position = Projection * View * Model * Pos;
}