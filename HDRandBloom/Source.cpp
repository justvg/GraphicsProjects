#include <vector>

#include "src\graphics\window.h"
#include "src\graphics\shader.h"
#include "src\math\vec.h"
#include "src\math\mat4.h"
#include "src\utils\utils.h"

using namespace core;
using namespace graphics;
using namespace math;

#define ArrayCount(A) (sizeof(A)/sizeof(A[0]))

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	shader Shader("src/shaders/vertex.vert", "src/shaders/fragment.frag");
	shader LightSourceShader("src/shaders/LightSource.vert", "src/shaders/LightSource.frag");
	shader BlurShader("src/shaders/debugquad.vert", "src/shaders/blur.frag");
	shader HDRShader("src/shaders/debugquad.vert", "src/shaders/debugquad.frag");

	float CubeVertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	float QuadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	GLuint CubeVAO, CubeVBO;
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &CubeVBO);
	glBindVertexArray(CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(6*sizeof(float)));
	glBindVertexArray(0);

	GLuint WoodTexture = LoadTexture("wood.png", true);

	GLuint QuadVAO, QuadVBO;
	glGenVertexArrays(1, &QuadVAO);
	glGenBuffers(1, &QuadVBO);
	glBindVertexArray(QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(3*sizeof(float)));
	glBindVertexArray(0);

	GLuint HdrFBO;
	glGenFramebuffers(1, &HdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, HdrFBO);

	GLuint ColorBuffers[2];
	glGenTextures(2, ColorBuffers);
	for(uint32_t I = 0; I < 2; I++)
	{
		glBindTexture(GL_TEXTURE_2D, ColorBuffers[I]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window.Width, Window.Height, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + I, GL_TEXTURE_2D, ColorBuffers[I], 0);
	}

	GLuint DepthRBO;
	glGenRenderbuffers(1, &DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window.Width, Window.Height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRBO);
	
	GLuint Attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, Attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer isn't complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint PingPongFBO[2];
	GLuint PingPongBuffers[2];
	glGenFramebuffers(2, PingPongFBO);
	glGenTextures(2, PingPongBuffers);
	for(uint32_t I = 0; I < 2; I++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, PingPongFBO[I]);
		glBindTexture(GL_TEXTURE_2D, PingPongBuffers[I]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window.Width, Window.Height, 0, GL_RGB, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PingPongBuffers[I], 0);	

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer isn't complete!" << std::endl;
		}		
	}

	Shader.Enable();
	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 100.0f);

	std::vector<vec3> LightPositions;
    LightPositions.push_back(vec3(0.0f, 0.5f,  1.5f));
    LightPositions.push_back(vec3(-4.0f, 0.5f, -3.0f));
    LightPositions.push_back(vec3(3.0f, 0.5f,  1.0f));
    LightPositions.push_back(vec3(-0.8f, 2.4f, -1.0f));

    std::vector<vec3> LightColors;
    LightColors.push_back(vec3(5.0f, 5.0f, 5.0f));
    LightColors.push_back(vec3(10.0f, 0.0f, 0.0f));
    LightColors.push_back(vec3(0.0f, 0.0f, 15.0f));
    LightColors.push_back(vec3(0.0f, 5.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	glUniform1i(glGetUniformLocation(Shader.ShaderID, "DiffuseTexture"), 0);

	for(uint32_t I = 0; I < LightPositions.size(); I++)
	{
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, ("Lights[" + std::to_string(I) + "].PosWorld").c_str()), 1, &LightPositions[I].x);
		glUniform3fv(glGetUniformLocation(Shader.ShaderID, ("Lights[" + std::to_string(I) + "].Color").c_str()), 1, &LightColors[I].x);
	}

	LightSourceShader.Enable();
	glUniformMatrix4fv(glGetUniformLocation(LightSourceShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);

	BlurShader.Enable();
	glUniform1i(glGetUniformLocation(BlurShader.ShaderID, "Image"), 0);

	HDRShader.Enable();
	glUniform1i(glGetUniformLocation(HDRShader.ShaderID, "Scene"), 0);
	glUniform1i(glGetUniformLocation(HDRShader.ShaderID, "BloomBlur"), 1);
	glUniform1f(glGetUniformLocation(HDRShader.ShaderID, "Exposure"), 1.0f);

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

		float CurrentFrame = (float)glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
		Window.ProcessInput(DeltaTime);

		Shader.Enable();
		glBindFramebuffer(GL_FRAMEBUFFER, HdrFBO);
		glBindVertexArray(CubeVAO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 View = mat4::LookAt(Window.Camera.Position, Window.Camera.Position + Window.Camera.Front);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WoodTexture);
		mat4 Model = mat4::Translation(vec3(0.0f, -1.0f, 0.0f));
		Model = Model * mat4::Scale(12.5f, 0.5f, 12.5f);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		Model = mat4::Translation(vec3(0.0f, 1.5f, 0.0f));
		Model = Model * mat4::Scale(0.5f);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		Model = mat4::Translation(vec3(2.0f, 0.0f, 1.0f));
		Model = Model * mat4::Scale(0.5f);
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(-1.0f, -1.0f, 2.0f));
        Model = Model * mat4::Rotate(60.0f, Normalize(vec3(1.0f, 0.0f, 1.0f)));
		glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(0.0f, 2.7f, 4.0f));
        Model = Model * mat4::Rotate(23.0f, Normalize(vec3(1.0, 0.0, 1.0)));
        Model = Model * mat4::Scale(1.25f);
        glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(-2.0f, 1.0f, -3.0f));
        Model = Model * mat4::Rotate(124.0f, Normalize(vec3(1.0, 0.0, 1.0)));
        glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(-3.0f, 0.0f, 0.0f));
        Model = Model * mat4::Scale(0.5f);
        glUniformMatrix4fv(glGetUniformLocation(Shader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
        glDrawArrays(GL_TRIANGLES, 0, 36);

		LightSourceShader.Enable();
		glUniformMatrix4fv(glGetUniformLocation(LightSourceShader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		for (uint32_t I = 0; I < LightPositions.size(); I++)
		{
			Model = mat4::Translation(LightPositions[I]);
			Model = Model * mat4::Scale(0.25f);
        	glUniformMatrix4fv(glGetUniformLocation(LightSourceShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
			glUniform3fv(glGetUniformLocation(LightSourceShader.ShaderID, "LightColor"), 1, &LightColors[I].x);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		BlurShader.Enable();
		glBindVertexArray(QuadVAO);
		bool Horizontal = true, FirstIteration = true;
		uint32_t Amount = 10; 
		for(uint32_t I = 0; I < Amount; I++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, PingPongFBO[Horizontal]);
			glUniform1i(glGetUniformLocation(BlurShader.ShaderID, "Horizontal"), Horizontal);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, FirstIteration ? ColorBuffers[1] : PingPongBuffers[!Horizontal]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			Horizontal = !Horizontal;
			FirstIteration = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		HDRShader.Enable();
		glBindVertexArray(QuadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ColorBuffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, PingPongBuffers[!Horizontal]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}