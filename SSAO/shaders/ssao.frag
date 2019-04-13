#version 330 core

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D NoiseTexture;

uniform vec3 SampleKernel[64];
uniform mat4 Projection;

const int KernelSize = 64;
const float Radius = 1.0;
const float Bias = 0.025;

const vec2 NoiseScale = vec2(900.0/4.0, 540.0/4.0); // Screen = 900x540

void main()
{
	vec3 FragPosView = texture(gPos, TexCoords).xyz;
	vec3 Normal = texture(gNormal, TexCoords).xyz;
	vec3 RandomVec = normalize(texture(NoiseTexture, TexCoords*NoiseScale).xyz);

	vec3 Tangent = normalize(RandomVec - Normal * dot(RandomVec, Normal));
	vec3 Bitangent = cross(Normal, Tangent);
	mat3 TBN = mat3(Tangent, Bitangent, Normal);

	float Occlusion = 0.0;
	for(int I = 0; I < KernelSize; I++)
	{
		vec3 Sample = TBN * SampleKernel[I];
		Sample = FragPosView + Sample * Radius;

		vec4 Offset = vec4(Sample, 1.0);
		Offset = Projection * Offset; // To clip-space
		Offset.xyz /= Offset.w; // Perspective divide
		Offset.xyz = Offset.xyz * 0.5 + 0.5;

		float SampledDepth = texture(gPos, Offset.xy).z;
		float RangeCheck = smoothstep(0.0, 1.0, Radius / abs(FragPosView.z - SampledDepth)); 
		Occlusion += (SampledDepth >= Sample.z + Bias ? 1.0 : 0.0) * RangeCheck; 
	}

	Occlusion = 1.0 - (Occlusion / KernelSize);
	FragColor = Occlusion;
}