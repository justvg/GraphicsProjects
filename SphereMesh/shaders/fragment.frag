#version 330 core

out vec4 FragColor;

uniform vec3 CamPosWorld;

in vec3 PosWorld;

void main()
{
	float Intensity = 1 / length(CamPosWorld - PosWorld);
	FragColor = Intensity * vec4(1.0, 0.0, 0.0, 1.0);
}