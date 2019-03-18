#include "camera.h"

namespace core {
namespace camera {

	camera::camera()
	{
		Position = vec3(0.0f, 0.0f, 3.0f);
		Front = vec3(0.0f, 0.0f, -1.0f);
		UpAxis = vec3(0.0f, 1.0f, 0.0f);

		MovSpeed = 10.0f;
		RotSensitivity = 0.1f;

		Pitch = 0.0f;
		Head = -90.0f;
	}

}}