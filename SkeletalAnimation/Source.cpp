#include <vector>

#include "src\graphics\window.h"
#include "src\graphics\shader.h"
#include "src\math\vec.h"
#include "src\math\mat4.h"

#include "src\mesh.h"

using namespace core;
using namespace graphics;
using namespace math;

#define ArrayCount(A) (sizeof(A)/sizeof(A[0]))

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.3f, 0.3f, 0.7f, 1.0f);

	shader Shader("src/shaders/skinning.vert", "src/shaders/fragment.frag");

	mesh BobLamp;
	BobLamp.LoadMesh("bob/boblampclean.md5mesh");

	Shader.Enable();
	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);

	std::vector<mat4> BoneTransforms;

	float StartTime = (float)glfwGetTime();
	float LastTime = (float)glfwGetTime();
 	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;
	int FrameNumb = 0;
	glDisable(GL_CULL_FACE);
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

		Shader.Enable();
		mat4 View = mat4::LookAt(Window.Camera.Position, Window.Camera.Position + Window.Camera.Front);
		mat4 Model = mat4::Scale(0.1f);
		Model = mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * Model;
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);

		float RunningTime = (float)glfwGetTime() - StartTime;
		BobLamp.BoneTransform(RunningTime, BoneTransforms);
		for(uint32_t I = 0; I < BoneTransforms.size(); I++)
		{
			glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, ("Bones[" + std::to_string(I) + "]").c_str()), 1, GL_FALSE, BoneTransforms[I].Elements);
		}
		BobLamp.Render(Shader.ShaderID);

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}