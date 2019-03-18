#pragma once

#include <math.h>
#define PI 3.14159265358979323846

namespace core {
namespace math {

	struct vec2
	{
		float x, y;
		
		vec2();
		vec2(float X, float Y);
		
		friend vec2 operator*(float A, vec2 B);
		friend vec2 operator*(vec2 A, float B);

		friend vec2 operator+(vec2 A, vec2 B);
		friend vec2 operator-(vec2 A, vec2 B);

		vec2& operator+=(vec2 B);
		vec2& operator-=(vec2 B);
	};

	struct vec3
	{
		float x, y, z;
		
		vec3();
		vec3(float X, float Y, float Z);

		friend vec3 operator*(float A, vec3 B);
		friend vec3 operator*(vec3 A, float B);
		
		friend vec3 operator+(vec3 A, vec3 B);
		friend vec3 operator-(vec3 A, vec3 B);

		friend float DotProduct(vec3 A, vec3 B);
		friend float LengthSq(vec3 A);
		friend float Length(vec3 A);
		friend vec3 Normalize(vec3 A);
		friend vec3 CrossProduct(vec3 A, vec3 B);

		void Normalize();

		vec3& operator+=(vec3 B);
		vec3& operator-=(vec3 B);
	};

	struct vec4
	{
		union 
		{
			struct 
			{
				float x, y, z, w;
			};
			float E[4];
		};
		
		vec4();
		vec4(float X, float Y, float Z, float W);

		friend vec4 operator*(float A, vec4 B);
		friend vec4 operator*(vec4 A, float B);
		
		friend vec4 operator+(vec4 A, vec4 B);
		friend vec4 operator-(vec4 A, vec4 B);

		vec4& operator+=(vec4 B);
		vec4& operator-=(vec4 B);
	};

}}