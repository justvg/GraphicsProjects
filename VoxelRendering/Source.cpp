#include <vector>

#include "src\graphics\window.h"
#include "src\graphics\shader.h"
#include "src\math\vec.h"
#include "src\math\mat4.h"

#define STB_IMAGE_IMPLEMENTATION
#include "src\utils\stb_image.h"
#include "src\utils\utils.h"

#include "SimplexNoise.cpp"

using namespace core;
using namespace graphics;
using namespace math;

#define ArrayCount(A) (sizeof(A)/sizeof(A[0]))

#define SIZE 100
#define GetVoxel(x, y, z) VoxelVolume[(x) + (y)*SIZE + (z)*SIZE*SIZE]
#define SetVoxel(x, y, z, value) VoxelVolume[(x) + (y)*SIZE + (z)*SIZE*SIZE] = value;

struct vertex
{
	vec3 Pos;
    vec3 Normal;
};

void AddQuad(std::vector<vertex> &Vertices, vertex A, vertex B, vertex C, vertex D)
{
	Vertices.push_back(A);
	Vertices.push_back(B);
	Vertices.push_back(C);
	Vertices.push_back(C);
	Vertices.push_back(B);
	Vertices.push_back(D);
}

struct voxel
{
	bool NotEmpty;
};

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	shader Shader("src/shaders/vertex.vert", "src/shaders/fragment.frag");

	voxel VoxelVolume[SIZE * SIZE * SIZE] = {};
	std::vector<vertex> Vertices; 						
	for(int ZI = 0; ZI < SIZE; ZI++)
	{
		for(int YI = 0; YI < SIZE; YI++)
		{
			for(int XI = 0; XI < SIZE; XI++)
			{
                float X = (float)XI/SIZE;
                float Y = (float)YI/SIZE;
                float Z = (float)ZI/SIZE;
                float NoiseValue = SimplexNoise(2, X, Y, Z);
                GetVoxel(XI, YI, ZI).NotEmpty = NoiseValue > 2.0f ? true : false;
            }
        }
    }

    vec3 LeftFaceNormal = vec3(-1, 0, 0);
    vec3 RightFaceNormal = vec3(1, 0, 0);
    vec3 FrontFaceNormal = vec3(0, 0, 1);
    vec3 BackFaceNormal = vec3(0, 0, -1);
    vec3 BottomFaceNormal = vec3(0, -1, 0);
    vec3 TopFaceNormal = vec3(0, 1, 0);
	for(int ZI = 0; ZI < SIZE; ZI++)
	{
		for(int YI = 0; YI < SIZE; YI++)
		{
			for(int XI = 0; XI < SIZE; XI++)
			{
				if(GetVoxel(XI, YI, ZI).NotEmpty)
				{
					float X = (float)XI;
					float Y = (float)YI;
					float Z = (float)-ZI;
					vec3 BaseVertex = vec3(X, Y, Z); // Low-left pos. of the cube
					vertex V1, V2, V3, V4;
					
					// Set positions left
					if((XI == 0) || (!GetVoxel(XI-1, YI, ZI).NotEmpty))
					{
						V1.Pos = BaseVertex;
						V2.Pos = vec3(X, Y+1, Z);
						V3.Pos = vec3(X, Y, Z-1);
						V4.Pos = vec3(X, Y+1, Z-1);
                        V1.Normal = V2.Normal = V3.Normal = V4.Normal = LeftFaceNormal;
						AddQuad(Vertices, V1, V2, V3, V4);
					}

					// Set positions right
					if((XI == SIZE-1) || (!GetVoxel(XI+1, YI, ZI).NotEmpty))
					{
						V1.Pos = vec3(X+1, Y, Z);
						V2.Pos = vec3(X+1, Y, Z-1);
						V3.Pos = vec3(X+1, Y+1, Z);
						V4.Pos = vec3(X+1, Y+1, Z-1);
                        V1.Normal = V2.Normal = V3.Normal = V4.Normal = RightFaceNormal;
						AddQuad(Vertices, V1, V2, V3, V4);
					}
					
					// Set positions front
					if((ZI == 0) || (!GetVoxel(XI, YI, ZI-1).NotEmpty))
					{
						V1.Pos = BaseVertex;
						V2.Pos = vec3(X+1, Y, Z);
						V3.Pos = vec3(X, Y+1, Z);
						V4.Pos = vec3(X+1, Y+1, Z);
                        V1.Normal = V2.Normal = V3.Normal = V4.Normal = FrontFaceNormal;
						AddQuad(Vertices, V1, V2, V3, V4);
					}

					// Set positions back
					if((ZI == SIZE-1) || (!GetVoxel(XI, YI, ZI+1).NotEmpty))
					{
						V1.Pos = vec3(X, Y, Z-1);
						V2.Pos = vec3(X, Y+1, Z-1);
						V3.Pos = vec3(X+1, Y, Z-1);
						V4.Pos = vec3(X+1, Y+1, Z-1);
                        V1.Normal = V2.Normal = V3.Normal = V4.Normal = BackFaceNormal;
						AddQuad(Vertices, V1, V2, V3, V4);
					}

					// Set positions bottom
					if((YI == 0) || (!GetVoxel(XI, YI-1, ZI).NotEmpty))
					{
						V1.Pos = BaseVertex;
						V2.Pos = vec3(X, Y, Z-1);
						V3.Pos = vec3(X+1, Y, Z);
						V4.Pos = vec3(X+1, Y, Z-1);
                        V1.Normal = V2.Normal = V3.Normal = V4.Normal = BottomFaceNormal;
						AddQuad(Vertices, V1, V2, V3, V4);
					}

					// Set positions top
					if((YI == SIZE - 1) || (!GetVoxel(XI, YI+1, ZI).NotEmpty))
					{
						V1.Pos = vec3(X, Y+1, Z);
						V2.Pos = vec3(X+1, Y+1, Z);
						V3.Pos = vec3(X, Y+1, Z-1);
						V4.Pos = vec3(X+1, Y+1, Z-1);
                        V1.Normal = V2.Normal = V3.Normal = V4.Normal = TopFaceNormal;
						AddQuad(Vertices, V1, V2, V3, V4);
					}
				}
			}
		}
	}

	Shader.Enable();

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(vertex), &Vertices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(3*sizeof(float)));

	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 300.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	mat4 Model = mat4::Scale(1.0f);
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
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, "CamPosWorld"), 1, &Window.Camera.Position.x);

		glDrawArrays(GL_TRIANGLES, 0, Vertices.size());

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}
