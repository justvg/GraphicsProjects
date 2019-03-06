#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

uniform samplerCube Skybox;

void main()
{
	FragColor = texture(Skybox, TexCoord);
}