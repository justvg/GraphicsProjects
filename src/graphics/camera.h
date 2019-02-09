#pragma once

#include "../math/vec.h"

using namespace core;
using namespace math;

namespace core {
namespace camera {

	class camera
	{
		public:
			camera();

			vec3 Position;
			vec3 Front;
			vec3 UpAxis; // NOTE(george): usually its value is (0.0f, 1.0f, 0.0f)

			float Pitch;
			float Head;
	};

}}