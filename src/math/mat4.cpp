#include "mat4.h"

namespace core {
namespace math {

	mat4::mat4()
	{
		for(int i = 0; i < MATRIX_SIZE; i++)
		{
			Elements[i] = 0.0f;
		}
	}

	mat4 mat4::Identity(float Diagonal)
	{
		mat4 Result;

		Result.Elements[0] = Diagonal;
		Result.Elements[1 + 1 * 4] = Diagonal;
		Result.Elements[2 + 2 * 4] = Diagonal;
		Result.Elements[3 + 3 * 4] = Diagonal;
	
		return (Result);
	}

	mat4 mat4::Translation(vec3 Translate)
	{
		mat4 Result = Identity(1.0f);

		Result.Elements[0 + 3*4] = Translate.x;
		Result.Elements[1 + 3*4] = Translate.y;
		Result.Elements[2 + 3*4] = Translate.z;

		return (Result);	
	}

	mat4 mat4::Rotate(float Angle, vec3 Axis)
	{
		mat4 Result;

		float Radians = Angle * PI / 180.0f;
		float Cos = cos(Radians);
		float Sin = sin(Radians);
		Axis.Normalize();

		Result.Elements[0 + 0*4] = Axis.x*Axis.x*(1.0f-Cos) + Cos;
		Result.Elements[0 + 1*4] = Axis.x*Axis.y*(1.0f-Cos) - Axis.z*Sin;
		Result.Elements[0 + 2*4] = Axis.x*Axis.z*(1.0f-Cos) + Axis.y*Sin;

		Result.Elements[1 + 0*4] = Axis.x*Axis.y*(1.0f-Cos) + Axis.z*Sin;
		Result.Elements[1 + 1*4] = Axis.y*Axis.y*(1.0f-Cos) + Cos;
		Result.Elements[1 + 2*4] = Axis.y*Axis.z*(1.0f-Cos) - Axis.x*Sin;

		Result.Elements[2 + 0*4] = Axis.x*Axis.z*(1.0f-Cos) - Axis.y*Sin;
		Result.Elements[2 + 1*4] = Axis.y*Axis.z*(1.0f-Cos) + Axis.x*Sin;
		Result.Elements[2 + 2*4] = Axis.z*Axis.z*(1.0f-Cos) + Cos;

		Result.Elements[3 + 3*4] = 1.0f;

		return(Result);
	}
	
	mat4 mat4::LookAt(vec3 From, vec3 Target, vec3 UpAxis)
	{
		vec3 Forward = Normalize(From - Target);

		vec3 Right = Normalize(CrossProduct(UpAxis, Forward));

		vec3 Up = Normalize(CrossProduct(Forward, Right));

		mat4 Result;
		Result.Elements[0 + 0*4] = Right.x;
		Result.Elements[0 + 1*4] = Right.y;
		Result.Elements[0 + 2*4] = Right.z;
		
		Result.Elements[1 + 0*4] = Up.x;
		Result.Elements[1 + 1*4] = Up.y;
		Result.Elements[1 + 2*4] = Up.z;
		
		Result.Elements[2 + 0*4] = Forward.x;
		Result.Elements[2 + 1*4] = Forward.y;
		Result.Elements[2 + 2*4] = Forward.z;
		
		Result.Elements[0 + 3*4] = -DotProduct(From, Right);
		Result.Elements[1 + 3*4] = -DotProduct(From, Up);
		Result.Elements[2 + 3*4] = -DotProduct(From, Forward);
		Result.Elements[3 + 3*4] = 1.0f;

		return(Result);
	}

	mat4 mat4::Perspective(float FoV, float ImageAspectRation, float Near, float Far)
	{
		float Scale = tanf(FoV * 0.5f * PI  / 180.0f) * Near;
		float Right = ImageAspectRation*Scale; 
		float Left = -Right;
		float Top = Scale;
		float Bottom = -Top;

		mat4 Result;
		Result.Elements[0 + 0*4] = 2*Near/(Right - Left);
		Result.Elements[1 + 1*4] = 2*Near/(Top - Bottom);
		Result.Elements[0 + 2*4] = (Right+Left)/(Right-Left);
		Result.Elements[1 + 2*4] = (Top+Bottom)/(Top-Bottom);
		Result.Elements[2 + 2*4] = -(Far+Near)/(Far-Near);
		Result.Elements[3 + 2*4] = -1;
		Result.Elements[2 + 3*4] = -(2*Far*Near)/(Far-Near);

		return(Result);
	}

	mat4 operator*(mat4 A, mat4 B)
	{
		mat4 Result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float Sum = 0.0f;
				for (int e = 0; e < 4; e++)
				{
					Sum += A.Elements[j + e*4] * B.Elements[e + i*4];
				}
				Result.Elements[j + i * 4] = Sum;
			}
		}

		return(Result);
	}

}}