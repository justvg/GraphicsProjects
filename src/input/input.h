#pragma once

#include "GLFW/glfw3.h"

#define MAX_KEYS 1000

namespace core {
namespace input {

	class input
	{
		private:
			bool Keys[MAX_KEYS];
			double X, Y;

		public:
			input();
			void InitPointer(GLFWwindow *Window);

			bool IsKeyPressed(int KeyCode);
			void GetMousePosition(double &X, double &Y);

		private:
			friend void KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods);
			friend void MousePositionCallback(GLFWwindow* Window, double XPos, double YPos);
	};

}}