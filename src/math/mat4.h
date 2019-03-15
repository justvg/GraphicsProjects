#pragma once

#include "vec.h"

#define MATRIX_SIZE 4*4

namespace core {
namespace math {

	struct mat4
	{
		float Elements[MATRIX_SIZE];
		mat4();
		
		static mat4 Identity(float Diagonal);
		static mat4 Translation(vec3 Translate);
		static mat4 Scale(float UniformScale);
		static mat4 Rotate(float Angle, vec3 Axis);
		static mat4 LookAt(vec3 From, vec3 Target, vec3 UpAxis = vec3(0.0f, 1.0f, 0.0f));
		static mat4 Ortho(float Bottom, float Top, float Left, float Right, float Near, float Far);
		static mat4 Perspective(float FoV, float ImageAspectRation, float Near, float Far);

		friend mat4 operator*(mat4 A, mat4 B);
	};

}}