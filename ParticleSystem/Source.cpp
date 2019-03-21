#include <vector>
#include <algorithm>

#include "src\graphics\window.h"
#include "src\graphics\shader.h"
#include "src\math\vec.h"
#include "src\math\mat4.h"
#include "src/graphics/particle_generator.h"

#define STB_IMAGE_IMPLEMENTATION
#include "src\utils\stb_image.h"
#include "src\utils\utils.h"

using namespace core;
using namespace graphics;
using namespace math;

#define ArrayCount(A) (sizeof(A)/sizeof(A[0]))

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	shader ParticleShader("src/shaders/particle.vert", "src/shaders/particle.frag");

	GLuint ParticleTexture = LoadTexture("Particle.png");

	vec3 ParticleGeneratorPos = vec3(0.0f, 2.0f, 0.0f);
	particle_generator ParticleGenerator(ParticleGeneratorPos, 1500, ParticleShader, ParticleTexture);

	ParticleShader.Enable();
	glUniformMatrix4fv(glGetUniformLocation(ParticleShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	glUniform1i(glGetUniformLocation(ParticleShader.ShaderID, "Texture"), 0);

	float LastTime = (float)glfwGetTime();
 	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;
	int FrameNumb = 0;
	while(!Window.Closed())
	{
		float CurrentTime = (float)glfwGetTime();
		FrameNumb++;
		if(CurrentTime - LastTime >= 1.0f)
		{
			std::cout << 1000.0f / (float)FrameNumb << std::endl;
			FrameNumb = 0;
			LastTime += 1.0f;
		}			

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float CurrentFrame = (float)glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
		Window.ProcessInput(DeltaTime);

		mat4 View = mat4::LookAt(Window.Camera.Position, Window.Camera.Position + Window.Camera.Front);
		ParticleGenerator.Update(DeltaTime, 1000.0f, Window.Camera.Position);
		ParticleGenerator.Draw(View);

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}