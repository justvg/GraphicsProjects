#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Scene;
uniform sampler2D BloomBlur;
uniform float Exposure = 1.0;

void main()
{
	vec3 HDRColor = texture(Scene, TexCoords).rgb;
	vec3 BloomColor = texture(BloomBlur, TexCoords).rgb;
	HDRColor += BloomColor;
	vec3 LDRColor = vec3(1.0) - exp(-HDRColor * Exposure);

	float Gamma = 2;
	LDRColor = pow(LDRColor, vec3(1.0 / Gamma));
	FragColor = vec4(LDRColor, 1.0);
}
