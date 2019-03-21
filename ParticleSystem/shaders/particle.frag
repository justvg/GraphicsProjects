#version 330 core

out vec4 FragColor;

in vec4 ParticleColor;
in vec2 TexCoord;

uniform sampler2D Texture;

void main()
{
	vec4 TextureColor = texture(Texture, TexCoord) * ParticleColor;
	FragColor = TextureColor;
}