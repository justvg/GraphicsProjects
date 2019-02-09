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

struct mesh 
{
	std::vector<vec3> Vertices;
	std::vector<uint32_t> Indices;

	void AddTriangle(uint32_t A, uint32_t B, uint32_t C);
	void AddQuad(uint32_t A, uint32_t B, uint32_t C, uint32_t D);
};

void mesh::AddTriangle(uint32_t A, uint32_t B, uint32_t C)
{
	Indices.push_back(A);
	Indices.push_back(B);
	Indices.push_back(C);
}

void mesh::AddQuad(uint32_t A, uint32_t B, uint32_t C, uint32_t D)
{
	Indices.push_back(A);
	Indices.push_back(B);
	Indices.push_back(C);
	Indices.push_back(A);
	Indices.push_back(C);
	Indices.push_back(D);
}

void MakeSphere(int Slices, int Stacks, mesh &Mesh)
{
	Mesh.Vertices.push_back(vec3(0.0f, 1.0f, 0.0f));
	for(int I = 0; I < Stacks - 1; I++)
	{
		float Phi = PI * (float)(I + 1) / (float)Stacks;
		float SinPhi = std::sinf(Phi);
		float CosPhi = std::cosf(Phi);
		for(int J = 0; J < Slices; J++)
		{
			float Azimuth = 2.0f*PI*(float)J / (float)Slices;
			float SinAzimuth = std::sinf(Azimuth);
			float CosAzimuth = std::cosf(Azimuth);
			float X = SinPhi * CosAzimuth;
			float Y = CosPhi;
			float Z = SinPhi * SinAzimuth;
			Mesh.Vertices.push_back(vec3(X, Y, Z));
		}
	}
	Mesh.Vertices.push_back(vec3(0.0f, -1.0f, 0.0f));

	for(int I = 0; I < Slices; I++)
	{
		uint32_t A = I + 1;
		uint32_t B = (I + 1) % Slices + 1;
		Mesh.AddTriangle(0, B, A);
	}

	for(int I = 0; I < Stacks - 2; I++)
	{	
		uint32_t AStart = I * Slices + 1;
		uint32_t BStart = (I+1)*Slices + 1;
		for(int J = 0; J < Slices; J++)
		{
			uint32_t A0 = AStart + J;
			uint32_t A1 = AStart + (J + 1) % Slices;
			uint32_t B0 = BStart + J;
			uint32_t B1 = BStart + (J + 1) % Slices;
			Mesh.AddQuad(A0, A1, B1, B0);
		}
	}

	for(int I = 0; I < Slices; I++)
	{
		uint32_t A = I + (Stacks - 2)*Slices + 1;
		uint32_t B = (I + 1) % Slices + (Stacks - 2)*Slices + 1;
		Mesh.AddTriangle(Mesh.Vertices.size() - 1, A, B);
	}
}

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	shader Shader("src/shaders/vertex.vert", "src/shaders/fragment.frag");
	Shader.Enable();

	mesh Mesh;
	MakeSphere(30, 15, Mesh);

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Mesh.Vertices.size() * sizeof(vec3), &Mesh.Vertices.front(), GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh.Indices.size() * sizeof(uint32_t), &Mesh.Indices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);

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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, Mesh.Indices.size(), GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}
