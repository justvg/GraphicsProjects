#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 ParticleColor;
out vec2 TexCoord;

uniform vec3 CameraUpWorld;
uniform vec3 CameraRightWorld;
uniform vec3 ParticleCenterWorld;
uniform vec2 ParticleSize;
uniform vec4 Color;

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);

void main()
{
	ParticleColor = Color;
	TexCoord = aTexCoord;

	vec3 VertexPosWorld = ParticleCenterWorld + 
						  CameraRightWorld * aPos.x * ParticleSize.x + 
						  CameraUpWorld * aPos.y * ParticleSize.y;

	gl_Position = Projection * View * vec4(VertexPosWorld, 1.0);
}