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
	std::vector<vec3> Normals;
	std::vector<uint32_t> Indices;

	void AddTriangle(uint32_t A, uint32_t B, uint32_t C);
};

void mesh::AddTriangle(uint32_t A, uint32_t B, uint32_t C)
{
	Indices.push_back(A);
	Indices.push_back(B);
	Indices.push_back(C);
}

struct terrain
{
	uint32_t Width, Depth;
	float Step;
	float *Heights;

	mesh Mesh;
};

void InitTerrain(terrain *Terrain, uint32_t Width, uint32_t Depth, float Step)
{
	Terrain->Width = Width;
	Terrain->Depth = Depth;
	Terrain->Step = Step;

	Terrain->Mesh.Vertices = std::vector<vec3>();
	Terrain->Mesh.Normals = std::vector<vec3>();
	Terrain->Mesh.Indices = std::vector<uint32_t>();

	Terrain->Heights = new float[Width * Depth];
}

void SetHeightsFromTexture(terrain *Terrain, char *TexturePath, float Scale)
{
	int ImgWidth, ImgHeight, nrChannels;
	uint8_t *Data = stbi_load(TexturePath, &ImgWidth, &ImgHeight, &nrChannels, 0);

	int Pitch = ImgWidth * 3;

	float ScaleX = (float)ImgWidth / (Terrain->Width - 1);
	float ScaleZ = (float)ImgHeight / (Terrain->Depth - 1);

	for(int Z = 0; Z < Terrain->Depth; Z++)
	{
		for(int X = 0; X < Terrain->Width; X++)
		{
			int ImgX = (int)(X * ScaleX);
			int ImgY = (int)(Z * ScaleZ);
			float Height = Data[ImgY * Pitch + ImgX * 3];

			Height = Height / 255.0f * 2.0f - 1.0f;
			Height *= Scale;
			Terrain->Heights[Z*Terrain->Width + X] = Height;
		}
	}
}

vec3 CalculateNormal(terrain *Terrain, int X, int Z)
{
	if(X == 0)
	{
		X = 1;
	}
	if(X == Terrain->Width-1)
	{
		X = Terrain->Width-2;
	}
	if(Z == 0)
	{
		Z = 1;
	}
	if(Z == Terrain->Depth-1)
	{
		Z = Terrain->Depth-2;
	}

	float HeightLeft = Terrain->Heights[Z*Terrain->Width + X - 1];
	float HeightRight = Terrain->Heights[Z*Terrain->Width + X + 1];
	float HeightDown = Terrain->Heights[(Z-1)*Terrain->Width + X];
	float HeightUp = Terrain->Heights[(Z+1)*Terrain->Width + X];

	vec3 Normal = vec3(HeightLeft - HeightRight, 2.0f, HeightUp-HeightDown);
	Normal.Normalize();

	return(Normal);
}

void BuildTerrainMesh(terrain *Terrain)
{
	for(int Z = 0; Z < Terrain->Depth; Z++)
	{
		for(int X = 0; X < Terrain->Width; X++)
		{
			float Height = Terrain->Heights[Z*Terrain->Width + X];
			vec3 Vertex = vec3(X * Terrain->Step, Height, -(Z * Terrain->Step));
			vec3 Normal = CalculateNormal(Terrain, X, Z);

			Terrain->Mesh.Vertices.push_back(Vertex);
			Terrain->Mesh.Normals.push_back(Normal);
		}
	}

#if 0
	for(int Z = 0; Z < Terrain->Depth - 1; Z++)
	{
		uint32_t AStart = Z * Terrain->Depth;
		for(int X = 0; X < Terrain->Width - 1; X++)
		{
			uint32_t A = AStart + X;
			uint32_t B = AStart + X + 1;
			uint32_t C = AStart + Terrain->Width + X;
			uint32_t D = AStart + Terrain->Width + X + 1;
			Terrain->Mesh.AddTriangle(A, B, C);
			Terrain->Mesh.AddTriangle(C, B, D);
		}	
	}
#else
	for(int Z = 0; Z < Terrain->Depth - 1; Z++)
	{
		uint32_t Start = Z * Terrain->Depth;
		for(int X = 0; X < Terrain->Width; X++)
		{
			if(X == 0 && Z > 0)
			{
				uint32_t A = Start + X;
				Terrain->Mesh.Indices.push_back(A);
			}

			uint32_t A = Start + X;
			Terrain->Mesh.Indices.push_back(A);
			uint32_t B = Start + Terrain->Width + X;
			Terrain->Mesh.Indices.push_back(B);

			if(X == Terrain->Width - 1)
			{
				uint32_t B = Start + Terrain->Width + X;
				Terrain->Mesh.Indices.push_back(B);
			}			
		}
	}
#endif
}

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	shader Shader("src/shaders/vertex.vert", "src/shaders/fragment.frag");
	Shader.Enable();

	terrain Terrain;
	InitTerrain(&Terrain, 251, 251, 0.5f);
	SetHeightsFromTexture(&Terrain, "height_map.png", 12.0f);
	BuildTerrainMesh(&Terrain);

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (Terrain.Mesh.Vertices.size() + Terrain.Mesh.Normals.size())* sizeof(vec3), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Terrain.Mesh.Vertices.size() * sizeof(vec3), &Terrain.Mesh.Vertices.front());
	glBufferSubData(GL_ARRAY_BUFFER, Terrain.Mesh.Vertices.size() * sizeof(vec3),
					Terrain.Mesh.Normals.size() * sizeof(vec3), &Terrain.Mesh.Normals.front());
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Terrain.Mesh.Indices.size() * sizeof(uint32_t), &Terrain.Mesh.Indices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(Terrain.Mesh.Vertices.size() * sizeof(vec3)));

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
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Position"), 0.7f, 0.2f, 2.0f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Ambient"), 0.02f, 0.02f, 0.02f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Diffuse"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(Shader.ShaderID, "PointLights[0].Specular"), 0.2f, 0.2f, 0.2f);

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

		glDrawElements(GL_TRIANGLE_STRIP, Terrain.Mesh.Indices.size(), GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}
