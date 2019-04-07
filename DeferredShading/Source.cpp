#include <vector>

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

int main(void)
{
	window Window("WindowName", 900, 540);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	shader GeometryPassShader("src/shaders/vertex.vert", "src/shaders/fragment.frag");
	shader StencilPassShader("src/shaders/deferred_shading.vert", "src/shaders/stencil_pass.frag");
	shader LightingPassShader("src/shaders/deferred_shading.vert", "src/shaders/deferred_shading.frag");
	shader LightShader("src/shaders/LightSource.vert", "src/shaders/LightSource.frag");

	mesh Sphere;
	Sphere.LoadMesh("sphere.obj");

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

	GeometryPassShader.Enable();
	mat4 Projection = mat4::Perspective(45.0f, (float)Window.Width/(float)Window.Height, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(GeometryPassShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	glUniform1i(glGetUniformLocation(GeometryPassShader.ShaderID, "DiffuseTexture"), 0);

	StencilPassShader.Enable();
	glUniformMatrix4fv(glGetUniformLocation(StencilPassShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);

	LightingPassShader.Enable();
	glUniform1i(glGetUniformLocation(LightingPassShader.ShaderID, "gPos"), 0);	
	glUniform1i(glGetUniformLocation(LightingPassShader.ShaderID, "gNormals"), 1);	
	glUniform1i(glGetUniformLocation(LightingPassShader.ShaderID, "gAlbedo"), 2);	

	glUniformMatrix4fv(glGetUniformLocation(LightingPassShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	vec2 ScreenSize = vec2((float)Window.Width, (float)Window.Height);
	glUniform2fv(glGetUniformLocation(LightingPassShader.ShaderID, "ScreenSize"), 1, &ScreenSize.x);

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

	LightShader.Enable();
	glUniformMatrix4fv(glGetUniformLocation(LightShader.ShaderID, "Projection"), 1, GL_FALSE, Projection.Elements);
	
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
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

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


		// NOTE(georgy): Stencil and lighting pass of deferred shading
		glBindFramebuffer(GL_READ_FRAMEBUFFER, GBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, Window.Width, Window.Height, 0, 0, Window.Width, Window.Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glDepthMask(GL_FALSE);
		glEnable(GL_STENCIL_TEST);

		StencilPassShader.Enable();
		glUniformMatrix4fv(glGetUniformLocation(StencilPassShader.ShaderID, "View"), 1, GL_FALSE, View.Elements);

		LightingPassShader.Enable();
		glUniformMatrix4fv(glGetUniformLocation(LightingPassShader.ShaderID, "View"), 1, GL_FALSE, View.Elements);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		for (uint32_t I = 0; I < LightPositions.size(); I++)
		{
			const float Constant = 1.0f;
			const float Linear = 0.7f;
			const float Quadratic = 1.8f;
			float MaxBrightness = fmaxf(fmaxf(LightColors[I].x, LightColors[I].y), LightColors[I].z);
			float Radius = (-Linear + sqrt(Linear*Linear - 4.0f*Quadratic*(Constant - MaxBrightness * 256.0f / 5.0f))) /
							(2.0f * Quadratic);

			Model = mat4::Translation(LightPositions[I]);
			Model = Model * mat4::Scale(Radius);

			StencilPassShader.Enable();
			glDrawBuffer(GL_NONE);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

			glUniformMatrix4fv(glGetUniformLocation(StencilPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
			Sphere.Render(StencilPassShader.ShaderID);
			
			LightingPassShader.Enable();
			glDrawBuffer(GL_FRONT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, GBufferPos);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, GBufferNormals);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, GBufferAlbedo);

			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);

			glEnable(GL_CULL_FACE);	
			glCullFace(GL_FRONT);

			glUniformMatrix4fv(glGetUniformLocation(LightingPassShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
			glUniform3fv(glGetUniformLocation(LightingPassShader.ShaderID, "Light.PosWorld"), 1, &LightPositions[I].x);
			glUniform3fv(glGetUniformLocation(LightingPassShader.ShaderID, "Light.Color"), 1, &LightColors[I].x);
			glUniform1f(glGetUniformLocation(LightingPassShader.ShaderID, "Light.Linear"), Linear);
			glUniform1f(glGetUniformLocation(LightingPassShader.ShaderID, "Light.Quadratic"), Quadratic);
			Sphere.Render(LightingPassShader.ShaderID);
		}
		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
		glDisable(GL_STENCIL_TEST);		

#if 1
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glCullFace(GL_BACK);

		LightShader.Enable();
		glBindVertexArray(CubeVAO);
		glUniformMatrix4fv(glGetUniformLocation(LightShader.ShaderID, "View"), 1, GL_FALSE, View.Elements);
		for (uint32_t I = 0; I < LightPositions.size(); I++)
		{
			Model = mat4::Translation(LightPositions[I]);
			Model = Model * mat4::Scale(0.25f);
        	glUniformMatrix4fv(glGetUniformLocation(LightShader.ShaderID, "Model"), 1, GL_FALSE, Model.Elements);
			glUniform3fv(glGetUniformLocation(LightShader.ShaderID, "LightColor"), 1, &LightColors[I].x);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			// Sphere.Render(LightShader.ShaderID);
		}
#endif 

		glfwPollEvents();
		glfwSwapBuffers(Window.Window);
	}

	system("PAUSE");
	return(0);
}