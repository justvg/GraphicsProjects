#pragma once

#include "../math/vec.h"
#include "../math/mat4.h"
#include "shader.h"
#include <vector>
#include <algorithm>

using namespace core;
using namespace math;

namespace core {
namespace graphics {

	struct particle
	{
		vec3 Position, Velocity;
		vec4 Color;
		vec2 Size;
		float LifeTime;
		float CameraDistance;

		bool operator<(particle& Other)
		{
			return (this->CameraDistance > Other.CameraDistance);
		}
	};

	class particle_generator
	{
		public:
			particle_generator(vec3 Pos, uint32_t ParticlesCount, shader &Shader, GLuint Texture);

			void Update(float DeltaTime, float NewParticlesInSecond, vec3 CamPos);
			void Draw(mat4 &View);

		private:
			std::vector<particle> Particles;
			uint32_t ParticlesCount;
			uint32_t LastUsedParticle;

			vec3 Position;			

			shader Shader;
			GLuint VAO;
			GLuint Texture;

			uint32_t UnusedParticle();
			void InitParticle(particle &Particle);
	};

}
}