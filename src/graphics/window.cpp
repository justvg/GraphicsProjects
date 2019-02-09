#include "window.h"
#include "../input/input.h"

using namespace core;
using namespace input;

namespace core {
namespace graphics {

	void FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height)
	{
		glViewport(0, 0, Width, Height);
	}
	
	window::window(const char *Name, int Width, int Height)
	{
		this->Name = Name;
		this->Width = Width;
		this->Height = Height;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		Window = glfwCreateWindow(Width, Height, Name, 0, 0);
		Input.InitPointer(Window);

		glfwMakeContextCurrent(Window);
		glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
		glfwSetKeyCallback(Window, KeyCallback);
		glfwSetCursorPosCallback(Window, MousePositionCallback);
		glViewport(0, 0, Width, Height);

		glewInit();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	window::~window()
	{
		glfwTerminate();
	}

	void window::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void window::Render()
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(Window);
	}

	void window::ProcessInput(float DeltaTime)
	{
		if(Input.IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(Window, true);
		}

		float CameraSpeed = DeltaTime*2.0f;
		if(Input.IsKeyPressed(GLFW_KEY_W))
		{
			Camera.Position += CameraSpeed*Camera.Front;
		}
		if(Input.IsKeyPressed(GLFW_KEY_S))
		{
			Camera.Position -= CameraSpeed*Camera.Front;
		}
		if(Input.IsKeyPressed(GLFW_KEY_A))
		{
			Camera.Position -= CameraSpeed*Normalize(CrossProduct(Camera.Front, Camera.UpAxis));
		}
		if(Input.IsKeyPressed(GLFW_KEY_D))
		{
			Camera.Position += CameraSpeed*Normalize(CrossProduct(Camera.Front, Camera.UpAxis));
		}

		double X, Y;
		Input.GetMousePosition(X, Y);
		static float LastMouseX = 0.0f;
		static float LastMouseY = 0.0f;
		static bool FirstMouse = true;
		if(FirstMouse)
		{
			LastMouseX = (float)X;
			LastMouseY = (float)Y;
			FirstMouse = false;
		}

		float Sensitivity = 0.1f;
		float XOffset = (float)((X - LastMouseX)*Sensitivity);
		float YOffset = (float)((LastMouseY - Y)*Sensitivity);

		Camera.Pitch += YOffset;
		Camera.Head += XOffset;

		Camera.Pitch = Camera.Pitch > 89.0f ? 89.0f : Camera.Pitch;
		Camera.Pitch = Camera.Pitch < -89.0f ? -89.0f : Camera.Pitch;

		LastMouseX = (float)X;
		LastMouseY = (float)Y;

		float PitchRadians = Camera.Pitch * (float)PI / 180.0f;
		float HeadRadians = Camera.Head * (float)PI / 180.0f;
		Camera.Front.x = cosf(PitchRadians)*cosf(HeadRadians);
		Camera.Front.y = sinf(PitchRadians);
		Camera.Front.z = cosf(PitchRadians)*sinf(HeadRadians);
		Camera.Front.Normalize();
	}

	void window::MainLoop()
	{
		Clear();
		Render();
		Update();
	}

	bool window::Closed()
	{
		bool Result = (bool)glfwWindowShouldClose(Window);
		return(Result);
	}

}}