#version 330 core

out float FragColor;

in vec2 TexCoords;

uniform sampler2D SSAOInput;

void main()
{
	vec2 TexelSize = 1.0 / vec2(textureSize(SSAOInput, 0));
	float Result = 0.0;
	for(int X = -2; X < 2; X++)
	{
		for(int Y = -2; Y < 2; Y++)
		{
			vec2 Offset = TexelSize * vec2(float(X), float(Y));
			Result += texture(SSAOInput, TexCoords + Offset).r;
		}
	}
	FragColor = Result / (4.0 * 4.0);
}