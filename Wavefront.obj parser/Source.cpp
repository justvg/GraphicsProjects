#include <vector>

#include "src\graphics\window.h"
#include "src\graphics\shader.h"
#include "src\math\vec.h"
#include "src\math\mat4.h"

#define STB_IMAGE_IMPLEMENTATION
#include "src\utils\stb_image.h"
#include "src\utils\utils.h"

#include "src\utils\parser.h"

using namespace core;
using namespace graphics;
using namespace math;

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	shader Shader("src/shaders/vertex.vert", "src/shaders/fragment.frag");

	model RenderModel;
	std::vector<vertex> VertexBuffer;
	std::vector<uint32_t> IndexBuffer;
	InitializeSceneObjects("sponza.obj", RenderModel, VertexBuffer, IndexBuffer);

	Shader.Enable();
	// NOTE(georgy): Material 
	glUniform1i(glGetUniformLocation(Shader.ShaderID, "Material.Diffuse"), 0);
	glUniform1i(glGetUniformLocation(Shader.ShaderID, "Material.Specular"), 1);
	glUniform1f(glGetUniformLocation(Shader.ShaderID, "Material.Shininess"), 32.0f);

	// NOTE(george): Directional light
	vec3 LightDir = vec3(0.5f, 0.3f, -0.4f);
	glUniform3fv(glGetUniformLocation(Shader.ShaderID, "DirLight.Direction"), 1, &LightDir.x);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "DirLight.Ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "DirLight.Diffuse"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "DirLight.Specular"), 0.7f, 0.7f, 0.7f);

	// NOTE(george): Point lights
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Position"), 1.9f, 0.2f, 0.5f);	
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Ambient"), 0.02f, 0.02f, 0.02f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Diffuse"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Specular"), 0.2f, 0.2f, 0.2f);

	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 300.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	mat4 Model = mat4::Scale(0.1f);
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements); 

 	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;
	while(!Window.Closed())
	{
		float CurrentFrame = (float)glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
		Window.ProcessInput(DeltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		mat4 View = mat4::LookAt(Window.Camera.Position, Window.Camera.Position + Window.Camera.Front);
		mat4 Model = mat4::Translation(vec3(1.9f, 0.2f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "CamPosWorld"), 1, &Window.Camera.Position.x);

		for(uint32_t I = 0; I < RenderModel.Meshes.size(); I++)
		{
			RenderModel.Meshes[I].Draw();	
		}

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}
