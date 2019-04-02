#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Image;

uniform bool Horizontal;
float Weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 TexOffset = vec2(1.0) / textureSize(Image, 0);
	vec3 Result = texture(Image, TexCoords).rgb * Weights[0];

	if(Horizontal)
	{
		for(int I = 1; I < 5; I++)
		{
			Result += texture(Image, TexCoords + vec2(TexOffset.x * I, 0.0)).rgb * Weights[I];
			Result += texture(Image, TexCoords - vec2(TexOffset.x * I, 0.0)).rgb * Weights[I];
		}
	} 
	else
	{
		for(int I = 1; I < 5; I++)
		{
			Result += texture(Image, TexCoords + vec2(0.0, TexOffset.y * I)).rgb * Weights[I];
			Result += texture(Image, TexCoords - vec2(0.0, TexOffset.y * I)).rgb * Weights[I];
		}
	}

	FragColor = vec4(Result, 1.0);
}