#pragma once

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "camera.h"
#include "..\input\input.h"

using namespace core;

namespace core {
namespace graphics {

	class window
	{
		public:
			const char *Name;
			int Width;
			int Height;
			GLFWwindow *Window;

			camera::camera Camera;

			window(const char *Name, int Width, int Height);
			~window();

			void MainLoop();
			bool Closed();

			void ProcessInput(float DeltaTime);

			friend void FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height);

		private:
			input::input Input;

			void Clear();
			void Render();
			void Update();
	};

}}