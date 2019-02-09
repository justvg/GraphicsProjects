#include "input.h"
#include <iostream>

namespace core {
namespace input {

	input::input()
	{
		for(int i = 0; i < MAX_KEYS; i++)
		{
			Keys[i] = false;
		}
	}

	void input::InitPointer(GLFWwindow *Window)
	{
		glfwSetWindowUserPointer(Window, this);
	}

	bool input::IsKeyPressed(int KeyCode)
	{
		if((KeyCode < 0) || (KeyCode >= MAX_KEYS))
		{
			return(false);
		}

		bool Result = Keys[KeyCode];
		return(Result);
	}

	void input::GetMousePosition(double &X, double &Y)
	{
		X = this->X;
		Y = this->Y;
	}

	void KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods)
	{
		input *Input = (input *)glfwGetWindowUserPointer(Window);
	
		Input->Keys[Key] = (Action != GLFW_RELEASE);
	}

	void MousePositionCallback(GLFWwindow* Window, double XPos, double YPos)
	{
		input *Input = (input *)glfwGetWindowUserPointer(Window);

		Input->X = XPos;
		Input->Y = YPos;
	}

}}