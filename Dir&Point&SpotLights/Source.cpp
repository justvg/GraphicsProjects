#include <vector>

#include "src\graphics\window.h"
#include "src\graphics\shader.h"
#include "src\math\vec.h"
#include "src\math\mat4.h"

#define STB_IMAGE_IMPLEMENTATION
#include "src\utils\stb_image.h"
#include "src\utils\utils.h"

using namespace core;
using namespace graphics;
using namespace math;

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	shader Shader("src/shaders/vertex.vert", "src/shaders/fragment.frag");
	Shader.Enable();

	float CubeVertices[] = {
		// Positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	vec3 CubePositions[] = {
		vec3( 0.0f, 0.0f, 0.0f),
		vec3( 2.0f, 5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3( 2.4f, -0.4f, -3.5f),
		vec3(-1.7f, 3.0f, -7.5f),
		vec3( 1.3f, -2.0f, -2.5f),
		vec3( 1.5f, 2.0f, -2.5f),
		vec3( 1.5f, 0.2f, -1.5f),
		vec3(-1.3f, 1.0f, -1.5f)
	};

	stbi_set_flip_vertically_on_load(true);
	GLuint TextureDiffuse = LoadTexture("container2.png");
	GLuint TextureSpecular = LoadTexture("container2_specular.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureSpecular);

	// NOTE(georgy): Material 
	glUniform1i(glGetUniformLocation(Shader.ShaderID, "Material.Diffuse"), 0);
	glUniform1i(glGetUniformLocation(Shader.ShaderID, "Material.Specular"), 1);
	glUniform1f(glGetUniformLocation(Shader.ShaderID, "Material.Shininess"), 32.0f);

	// NOTE(george): Directional light
	vec3 LightDir = vec3(0.5f, 0.3f, -0.4f);
	glUniform3fv(glGetUniformLocation(Shader.ShaderID, "DirLight.Direction"), 1, &LightDir.x);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "DirLight.Ambient"), 0.05f, 0.05f, 0.1f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "DirLight.Diffuse"), 0.2f, 0.2f, 0.7f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "DirLight.Specular"), 0.7f, 0.7f, 0.7f);

	// NOTE(george): Point lights
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Position"), 0.7f, 0.2f, 2.0f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Ambient"), 0.02f, 0.02f, 0.06f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Diffuse"), 0.2f, 0.2f, 0.6f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Specular"), 0.2f, 0.2f, 0.6f);

	// NOTE(george): Spotlight
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "SpotLight.Ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "SpotLight.Diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "SpotLight.Specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(Shader.ShaderID, "SpotLight.Penumbra"), cos(12.5f * PI / 180.0f));
	glUniform1f(glGetUniformLocation(Shader.ShaderID, "SpotLight.Umbra"), cos(15.0f * PI / 180.0f));  

	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);

	glUniform3f(glGetUniformLocation(Shader.ShaderID, "LightColor"), 1.0f, 1.0f, 1.0f);

 	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;
	while(!Window.Closed())
	{
		float CurrentFrame = (float)glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
		Window.ProcessInput(DeltaTime);

		mat4 View = mat4::LookAt(Window.Camera.Position, Window.Camera.Position + Window.Camera.Front);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "View"), 1, GL_FALSE, View.Elements);

		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "CamPosWorld"), 1, &Window.Camera.Position.x);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "SpotLight.Position"), 1, &Window.Camera.Position.x);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "SpotLight.Direction"), 1, &Window.Camera.Front.x);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(unsigned int i = 0; i < 10; i++)
		{
			float Angle = 20.0f * i;
			mat4 Model = mat4::Translation(CubePositions[i]);
			Model = Model * mat4::Rotate(Angle, vec3(1.0f, 0.3f, 0.5f)); 

			glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}
