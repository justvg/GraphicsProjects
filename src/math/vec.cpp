#include "vec.h"

namespace core {
namespace math {

	vec2::vec2() {}

	vec2::vec2(float X, float Y)
	{
		this->x = X;
		this->y = Y;
	}

	vec2 operator*(float A, vec2 B)
	{
		vec2 Result;
		Result.x = A * B.x;
		Result.y = A * B.y;

		return(Result);
	}

	vec2 operator*(vec2 A, float B)
	{
		vec2 Result;
		Result = B * A;

		return(Result);
	}

	vec2 operator+(vec2 A, vec2 B)
	{
		vec2 Result;
		Result.x = A.x + B.x;
		Result.y = A.y + B.y;

		return(Result);
	}

	vec2 operator-(vec2 A, vec2 B)
	{
		vec2 Result;
		Result.x = A.x - B.x;
		Result.y = A.y - B.y;

		return(Result);
	}

	vec2& vec2::operator+=(vec2 B)
	{
		*this = *this + B;

		return(*this);
	}

	vec2& vec2::operator-=(vec2 B)
	{
		*this = *this - B;

		return(*this);
	}	



	vec3::vec3() {}

	vec3::vec3(float X, float Y, float Z)
	{
		this->x = X;
		this->y = Y;
		this->z = Z;
	}

	vec3 operator*(float A, vec3 B)
	{
		vec3 Result;
		Result.x = A * B.x;
		Result.y = A * B.y;
		Result.z = A * B.z;

		return(Result);
	}

	vec3 operator*(vec3 A, float B)
	{
		vec3 Result;
		Result = B * A;

		return(Result);
	}

	vec3 operator+(vec3 A, vec3 B)
	{
		vec3 Result;
		Result.x = A.x + B.x;
		Result.y = A.y + B.y;
		Result.z = A.z + B.z;

		return(Result);
	}

	vec3 operator-(vec3 A, vec3 B)
	{
		vec3 Result;
		Result.x = A.x - B.x;
		Result.y = A.y - B.y;
		Result.z = A.z - B.z;

		return(Result);
	}

	vec3& vec3::operator+=(vec3 B)
	{
		*this = *this + B;

		return(*this);
	}

	vec3& vec3::operator-=(vec3 B)
	{
		*this = *this - B;

		return(*this);
	}

	float DotProduct(vec3 A, vec3 B)
	{
		float Result;
		Result = A.x*B.x + A.y*B.y + A.z*B.z;

		return (Result);
	}

	float LengthSq(vec3 A)
	{
		float Result;
		Result = DotProduct(A, A);

		return (Result);
	}

	float Length(vec3 A)
	{
		float Result;
		Result = sqrtf(LengthSq(A));

		return (Result);
	}

	void vec3::Normalize()
	{
		float Len = Length(*this);

		x /= Len;
		y /= Len;
		z /= Len;
	}

	vec3 Normalize(vec3 A)
	{
		vec3 Result;
		float InvLen = 1.0f/Length(A);

		Result = InvLen * A;	
		return (Result);
	}

	vec3 CrossProduct(vec3 A, vec3 B)
	{
		vec3 Result;
		Result.x = A.y*B.z - A.z*B.y;
		Result.y = A.z*B.x - A.x*B.z;
		Result.z = A.x*B.y - A.y*B.x;

		return(Result);
	}


	vec4::vec4() {}

	vec4::vec4(float X, float Y, float Z, float W)
	{
		this->x = X;
		this->y = Y;
		this->z = Z;
		this->w = W;
	}

	vec4 operator*(float A, vec4 B)
	{
		vec4 Result;
		Result.x = A * B.x;
		Result.y = A * B.y;
		Result.z = A * B.z;
		Result.w = A * B.w;

		return(Result);
	}

	vec4 operator*(vec4 A, float B)
	{
		vec4 Result;
		Result = B * A;

		return(Result);
	}

	vec4 operator+(vec4 A, vec4 B)
	{
		vec4 Result;
		Result.x = A.x + B.x;
		Result.y = A.y + B.y;
		Result.z = A.z + B.z;
		Result.w = A.w + B.w;

		return(Result);
	}

	vec4 operator-(vec4 A, vec4 B)
	{
		vec4 Result;
		Result.x = A.x - B.x;
		Result.y = A.y - B.y;
		Result.z = A.z - B.z;
		Result.w = A.w - B.w;

		return(Result);
	}

	vec4& vec4::operator+=(vec4 B)
	{
		*this = *this + B;

		return(*this);
	}

	vec4& vec4::operator-=(vec4 B)
	{
		*this = *this - B;

		return(*this);
	}

}}