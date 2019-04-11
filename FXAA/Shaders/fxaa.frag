#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Scene;
uniform vec2 InverseTextureSize;
uniform float FXAASpanMax;
uniform float FXAAReduceMin;
uniform float FXAAReduceMul;

void main()
{
	// vec2 TexCoordOffset = 1.0 / textureSize(Scene, 0);
	vec2 TexCoordOffset = InverseTextureSize;

	// NOTE(georgy): Sample texels
	vec3 Luma = vec3(0.299, 0.587, 0.114);
	float LumaCenter = dot(Luma, texture(Scene, TexCoords).rgb);
	float LumaTL = dot(Luma, texture(Scene, TexCoords + (vec2(-1.0, 1.0) * TexCoordOffset)).rgb);
	float LumaTR = dot(Luma, texture(Scene, TexCoords + (vec2(1.0, 1.0) * TexCoordOffset)).rgb);
	float LumaBL = dot(Luma, texture(Scene, TexCoords + (vec2(-1.0, -1.0) * TexCoordOffset)).rgb);
	float LumaBR = dot(Luma, texture(Scene, TexCoords + (vec2(1.0, -1.0) * TexCoordOffset)).rgb);

	// NOTE(georgy): Find sample direction
	vec2 Dir;
	Dir.x = ((LumaTL + LumaTR) - (LumaBL + LumaBR));
	Dir.y = (LumaTL + LumaBL) - (LumaTR + LumaBR);
	
	// NOTE(georgy): "Normalize" direction. Min of Dir.x, Dir.y ~ 1.0
	float DirReduce = max((LumaTL + LumaTR + LumaBL + LumaBR) * 0.25 * FXAAReduceMul, FXAAReduceMin);
	float InverseDirAdjustment = 1.0 / (min(abs(Dir.x), abs(Dir.y)) + DirReduce);

	Dir = min(vec2(FXAASpanMax, FXAASpanMax), max(vec2(-FXAASpanMax, -FXAASpanMax), Dir * InverseDirAdjustment));
	vec2 DirTextureSpace = Dir * TexCoordOffset;

	vec3 Result1 = (1.0 / 2.0) * 
					(texture(Scene, TexCoords + (DirTextureSpace * vec2(1.0/3.0 - 0.5))).rgb +
				   	texture(Scene, TexCoords + (DirTextureSpace * vec2(2.0/3.0 - 0.5))).rgb);

	vec3 Result2 = Result1 * (1.0 / 2.0) + (1.0/4.0) *
					(texture(Scene, TexCoords + (DirTextureSpace * vec2(0.0/3.0 - 0.5))).rgb +
					texture(Scene, TexCoords + (DirTextureSpace * vec2(3.0/3.0 - 0.5))).rgb);
 
	float LumaMin = min(LumaCenter, min(min(LumaTL, LumaTR), min(LumaBL, LumaBR)));
	float LumaMax = max(LumaCenter, max(max(LumaTL, LumaTR), max(LumaBL, LumaBR)));
	float LumaResult2 = dot(Luma, Result2);

	// NOTE(georgy): Check if we're sampling to far away
	if((LumaResult2 < LumaMin) || (LumaResult2 > LumaMax))
	{
		FragColor = vec4(Result1, 1.0);
	}
	else
	{
		FragColor = vec4(Result2, 1.0);
	}
}