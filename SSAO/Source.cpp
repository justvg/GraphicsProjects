#include <vector>
#include <random>

#include "src\graphics\window.h"
#include "src\graphics\shader.h"
#include "src\math\vec.h"
#include "src\math\mat4.h"
#include "src\utils\utils.h"

#include "src\mesh.h"

using namespace core;
using namespace graphics;
using namespace math;

#define ArrayCount(A) (sizeof(A)/sizeof(A[0]))

float Lerp(float A, float B, float t)
{
	float Result = A + t*(B - A);
	return(Result);
}

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	shader GeometryPassShader("src/shaders/vertex.vert", "src/shaders/fragment.frag");
	shader LightingPassShader("src/shaders/deferred_shading.vert", "src/shaders/deferred_shading.frag");
	shader SSAOShader("src/shaders/ssao.vert", "src/shaders/ssao.frag");
	shader SSAOBlurShader("src/shaders/ssao.vert", "src/shaders/ssaoblur.frag");

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

	GLuint GBuffer;
	glGenFramebuffers(1, &GBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffer);

	GLuint GBufferPos, GBufferNormals, GBufferAlbedo;
	glGenTextures(1, &GBufferPos);
	glBindTexture(GL_TEXTURE_2D, GBufferPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window.Width, Window.Height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GBufferPos, 0);

	glGenTextures(1, &GBufferNormals);
	glBindTexture(GL_TEXTURE_2D, GBufferNormals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window.Width, Window.Height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GBufferNormals, 0);

	glGenTextures(1, &GBufferAlbedo);
	glBindTexture(GL_TEXTURE_2D, GBufferAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window.Width, Window.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GBufferAlbedo, 0);

	GLuint DepthRBO;
	glGenRenderbuffers(1, &DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window.Width, Window.Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRBO);
	
	GLuint Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, Attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer isn't complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint SSAOFBO, SSAOColorBuffer;
	glGenFramebuffers(1, &SSAOFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, SSAOFBO);

	glGenTextures(1, &SSAOColorBuffer);
	glBindTexture(GL_TEXTURE_2D, SSAOColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Window.Width, Window.Height, 0, GL_RED, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAOColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer isn't complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint SSAOFBOBlur, SSAOColorBufferBlur;
	glGenFramebuffers(1, &SSAOFBOBlur);
	glBindFramebuffer(GL_FRAMEBUFFER, SSAOFBOBlur);

	glGenTextures(1, &SSAOColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, SSAOColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Window.Width, Window.Height, 0, GL_RED, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAOColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer isn't complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::uniform_real_distribution<float> RandomFloats(0.0f, 1.0f);
	std::default_random_engine Generator;
	std::vector<vec3> SSAOKernel;
	for(uint32_t I = 0; I < 64; I++)
	{
		vec3 Sample;
		Sample.x = RandomFloats(Generator) * 2.0f - 1.0f;
		Sample.y = RandomFloats(Generator) * 2.0f - 1.0f;
		Sample.z = RandomFloats(Generator);
		Sample.Normalize();
		Sample = Sample * RandomFloats(Generator);

		float Scale = (float) I / 64.0f;
		Scale = Lerp(0.1f, 1.0f, Scale * Scale);
		Sample = Sample * Scale;

		SSAOKernel.push_back(Sample);
	}

	std::vector<vec3> SSAONoise;
	for(uint32_t I = 0; I < 16; I++)
	{
		vec3 Noise;
		Noise.x = RandomFloats(Generator) * 2.0f - 1.0f;
		Noise.y = RandomFloats(Generator) * 2.0f - 1.0f;
		Noise.z = 0.0f;

		SSAONoise.push_back(Noise);
	}
	GLuint NoiseTexture;
	glGenTextures(1, &NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &SSAONoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GeometryPassShader.Enable();
	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	glUniform1i(glGetUniformLocation(GeometryPassShader.ShaderID, "DiffuseTexture"), 0);

	SSAOShader.Enable();
	glUniformMatrix4fv(glGetUniformLocation(SSAOShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	glUniform1i(glGetUniformLocation(SSAOShader.ShaderID, "gPos"), 0);	
	glUniform1i(glGetUniformLocation(SSAOShader.ShaderID, "gNormal"), 1);	
	glUniform1i(glGetUniformLocation(SSAOShader.ShaderID, "NoiseTexture"), 2);	
	for(uint32_t I = 0; I < 64; I++)
	{
		glUniform3fv(glGetUniformLocation(SSAOShader.ShaderID, ("SampleKernel[" + std::to_string(I) + "]").c_str()), 1, &SSAOKernel[I].x);
	}

	SSAOBlurShader.Enable();
	glUniform1i(glGetUniformLocation(SSAOBlurShader.ShaderID, "SSAOInput"), 0);		

	LightingPassShader.Enable();
	glUniform1i(glGetUniformLocation(LightingPassShader.ShaderID, "gPos"), 0);	
	glUniform1i(glGetUniformLocation(LightingPassShader.ShaderID, "gNormals"), 1);	
	glUniform1i(glGetUniformLocation(LightingPassShader.ShaderID, "gAlbedo"), 2);	
	glUniform1i(glGetUniformLocation(LightingPassShader.ShaderID, "SSAO"), 3);	

#if 1
	std::vector<vec3> LightPositions;
    LightPositions.push_back(vec3(0.0f, 0.5f,  1.5f));
    LightPositions.push_back(vec3(-4.0f, 0.5f, -3.0f));
    LightPositions.push_back(vec3(3.0f, 0.5f,  1.0f));
    LightPositions.push_back(vec3(-0.8f, 2.4f, -1.0f));

    std::vector<vec3> LightColors;
    LightColors.push_back(vec3(1.0f, 1.0f, 1.0f));
    LightColors.push_back(vec3(1.0f, 0.0f, 0.0f));
    LightColors.push_back(vec3(0.0f, 0.0f, 1.0f));
    LightColors.push_back(vec3(0.0f, 1.0f, 0.0f));
#endif

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

		// NOTE(georgy): Fill G-Buffer
		GeometryPassShader.Enable();
		glBindFramebuffer(GL_FRAMEBUFFER, GBuffer);
		glBindVertexArray(CubeVAO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 View = mat4::LookAt(Window.Camera.Position, Window.Camera.Position + Window.Camera.Front);
		glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WoodTexture);
		mat4 Model = mat4::Translation(vec3(0.0f, -1.0f, 0.0f));
		Model = Model * mat4::Scale(12.5f, 0.5f, 12.5f);
		glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		Model = mat4::Translation(vec3(0.0f, 1.5f, 0.0f));
		Model = Model * mat4::Scale(0.5f);
		glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		Model = mat4::Translation(vec3(2.0f, 0.0f, 1.0f));
		Model = Model * mat4::Scale(0.5f);
		glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(-1.0f, -1.0f, 2.0f));
        Model = Model * mat4::Rotate(60.0f, Normalize(vec3(1.0f, 0.0f, 1.0f)));
		glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
		glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(0.0f, 2.7f, 4.0f));
        Model = Model * mat4::Rotate(23.0f, Normalize(vec3(1.0, 0.0, 1.0)));
        Model = Model * mat4::Scale(1.25f);
        glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(-2.0f, 1.0f, -3.0f));
        Model = Model * mat4::Rotate(124.0f, Normalize(vec3(1.0, 0.0, 1.0)));
        glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Model = mat4::Translation(vec3(-3.0f, 0.0f, 0.0f));
        Model = Model * mat4::Scale(0.5f);
        glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
        glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindFramebuffer(GL_FRAMEBUFFER, SSAOFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		SSAOShader.Enable();
		glBindVertexArray(QuadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GBufferPos);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GBufferNormals);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, NoiseTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindFramebuffer(GL_FRAMEBUFFER, SSAOFBOBlur);
		glClear(GL_COLOR_BUFFER_BIT);
		SSAOBlurShader.Enable();
		glBindVertexArray(QuadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SSAOColorBuffer);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LightingPassShader.Enable();
		glBindVertexArray(QuadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GBufferPos);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GBufferNormals);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, SSAOColorBufferBlur);
		for(uint32_t I = 0; I < 4; I++)
		{
			vec4 LightPosView = View * vec4(LightPositions[I].x, LightPositions[I].y, LightPositions[I].z, 1.0f);
			glUniform3fv(glGetUniformLocation(LightingPassShader.ShaderID, ("Lights[" + std::to_string(I) + "].PosView").c_str()), 1, &LightPosView.x);
			glUniform3fv(glGetUniformLocation(LightingPassShader.ShaderID, ("Lights[" + std::to_string(I) + "].Color").c_str()), 1, &LightColors[I].x);
		}
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}