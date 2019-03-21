#include "particle_generator.h"

namespace core {
namespace graphics {

	particle_generator::particle_generator(vec3 Pos, uint32_t ParticlesCount, shader &Shader, GLuint Texture)
										: Shader(Shader), Texture(Texture), ParticlesCount(ParticlesCount), Position(Pos)
	{
		LastUsedParticle = 0;

		float Vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};

		GLuint VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(3*sizeof(float)));
		glBindVertexArray(0);

		for(uint32_t I = 0; I < ParticlesCount; I++)
		{
			particle Particle;
			Particle.LifeTime = -1.0f;
			Particles.push_back(Particle);
		}
	}

	uint32_t particle_generator::UnusedParticle()
	{
		for(uint32_t I = LastUsedParticle; I < Particles.size(); I++)
		{
			if(Particles[I].LifeTime <= 0.0f)
			{
				LastUsedParticle = I;
				return(I);
			}
		}

		for(uint32_t I = 0; I < LastUsedParticle; I++)
		{
			if(Particles[I].LifeTime <= 0.0f)
			{
				LastUsedParticle = I;
				return(I);
			}
		}

		LastUsedParticle = 0;
		return(0);
	}

	void particle_generator::InitParticle(particle &Particle)
	{
		Particle.LifeTime = 3.0f;
		Particle.Position = Position;
		
		vec3 MainDir = vec3(0.0f, 5.0f, 0.0f);	
		vec3 RandomDir = vec3(((rand() % 200) - 100.0f) / 100.0f, ((rand() % 200) - 100.0f) / 100.0f, ((rand() % 200) - 100.0f) / 100.0f);
		Particle.Velocity = MainDir + RandomDir;
		Particle.Color = vec4((rand() % 256) / 80, (rand() % 256) / 80, (rand() % 256) / 80, (rand() % 256) / 3);
		Particle.Size = vec2((rand() % 100) / 200.0f + 0.1f, (rand() % 100) / 300.0f + 0.1f);
	}

	void particle_generator::Update(float DeltaTime, float NewParticlesInSecond, vec3 CamPos)
	{
		uint32_t NewParticlesCount = (uint32_t)(DeltaTime * NewParticlesInSecond);
		if(NewParticlesCount > (uint32_t)(0.016f*NewParticlesInSecond))
		{
			NewParticlesCount = (uint32_t)(0.016f*NewParticlesInSecond);
		}
		for (uint32_t I = 0; I < NewParticlesCount; I++)
		{
			uint32_t FreeParticle = UnusedParticle();
			InitParticle(Particles[FreeParticle]);
		}

		for(uint32_t I = 0; I < ParticlesCount; I++)
		{
			particle &Particle = Particles[I];
			Particle.LifeTime -= DeltaTime;
			if(Particle.LifeTime > 0.0f)
			{
				Particle.Velocity += vec3(0.0f, -9.8f, 0.0f) * DeltaTime * 0.5f;
				Particle.Position += Particle.Velocity * DeltaTime;
				Particle.CameraDistance = LengthSq(CamPos - Particle.Position);
			}
			else
			{
				Particle.CameraDistance = -1.0f;
			}
		}

		std::sort(&Particles[0], &Particles[ParticlesCount-1]);
	}

	void particle_generator::Draw(mat4 &View)
	{
		Shader.Enable();
		glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		vec3 CameraRightWorld = vec3(View.Elements[0 + 0*4], View.Elements[0 + 1*4], View.Elements[0 + 2*4]);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "CameraRightWorld"), 1, &CameraRightWorld.x);
		vec3 CameraUpWorld = vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "CameraUpWorld"), 1, &CameraUpWorld.x);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		for(uint32_t I = 0; I < Particles.size(); I++)
		{
			if(Particles[I].LifeTime >= 0.0f)
			{
				glUniform3fv(glGetUniformLocation(Shader.ShaderID, "ParticleCenterWorld"), 1, &Particles[I].Position.x);
				glUniform4fv(glGetUniformLocation(Shader.ShaderID, "Color"), 1, &Particles[I].Color.x);
				glUniform2fv(glGetUniformLocation(Shader.ShaderID, "ParticleSize"), 1, &Particles[I].Size.x);
				glBindVertexArray(VAO);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Texture);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		glBindVertexArray(0);
		glDisable(GL_BLEND);
		Shader.Disable();
	}

}
}