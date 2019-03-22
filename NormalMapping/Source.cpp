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
	glClearColor(0.3f, 0.3f, 0.7f, 1.0f);

	shader Shader("src/shaders/vertex.vert", "src/shaders/fragment.frag");

	vec3 Pos1(-1.0f, 1.0f, 0.0f);
	vec3 Pos2(-1.0f, -1.0f, 0.0f);
	vec3 Pos3(1.0f, -1.0f, 0.0f);
	vec3 Pos4(1.0f, 1.0f, 0.0f);

	vec2 UV1(0.0f, 1.0f);
	vec2 UV2(0.0f, 0.0f);
	vec2 UV3(1.0f, 0.0f);
	vec2 UV4(1.0f, 1.0f);

	vec3 Normal(0.0f, 0.0f, 1.0f);

	vec3 Tangent1, Tangent2;

	vec3 Edge1 = Pos2 - Pos1;
	vec3 Edge2 = Pos3 - Pos1;
	vec2 DeltaUV1 = UV2 - UV1;
	vec2 DeltaUV2 = UV3 - UV1;

	Tangent1 = DeltaUV2.y * Edge1 - DeltaUV1.y * Edge2;
	Tangent1.Normalize();

	Edge1 = Pos3 - Pos1;
	Edge2 = Pos4 - Pos1;
	DeltaUV1 = UV3 - UV1;
	DeltaUV2 = UV4 - UV1;

	Tangent2 = DeltaUV2.y * Edge1 - DeltaUV1.y * Edge2;
	Tangent2.Normalize();

	float QuadVertices[] = {
		Pos1.x, Pos1.y, Pos1.z, Normal.x, Normal.y, Normal.z, UV1.x, UV1.y, Tangent1.x, Tangent1.y, Tangent1.z, 
		Pos2.x, Pos2.y, Pos2.z, Normal.x, Normal.y, Normal.z, UV2.x, UV2.y, Tangent1.x, Tangent1.y, Tangent1.z, 
		Pos3.x, Pos3.y, Pos3.z, Normal.x, Normal.y, Normal.z, UV3.x, UV3.y, Tangent1.x, Tangent1.y, Tangent1.z, 

		Pos1.x, Pos1.y, Pos1.z, Normal.x, Normal.y, Normal.z, UV1.x, UV1.y, Tangent2.x, Tangent2.y, Tangent2.z, 
		Pos3.x, Pos3.y, Pos3.z, Normal.x, Normal.y, Normal.z, UV3.x, UV3.y, Tangent2.x, Tangent2.y, Tangent2.z, 
		Pos4.x, Pos4.y, Pos4.z, Normal.x, Normal.y, Normal.z, UV4.x, UV4.y, Tangent2.x, Tangent2.y, Tangent2.z
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void *)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void *)(6*sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void *)(8*sizeof(float)));
	glBindVertexArray(0);

	GLuint DiffuseMap = LoadTexture("brickwall.jpg");
	GLuint NormalMap = LoadTexture("brickwall_normal.jpg");

	Shader.Enable();
	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	glUniform1i(glGetUniformLocation(Shader.ShaderID, "DiffuseMap"), 0);
	glUniform1i(glGetUniformLocation(Shader.ShaderID, "NormalMap"), 1);
	vec3 LightPosWorld = vec3(0.5f, 1.0f, 0.3f);
	glUniform3fv(glGetUniformLocation(Shader.ShaderID, "LightPosWorld"), 1, &LightPosWorld.x);

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

		Shader.Enable();
		mat4 Model = mat4::Rotate((float)glfwGetTime() * -10.0f, vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		mat4 View = mat4::LookAt(Window.Camera.Position, Window.Camera.Position + Window.Camera.Front);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "CamPosWorld"), 1, &Window.Camera.Position.x);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DiffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NormalMap);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}