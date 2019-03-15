#pragma once

#include "GL\glew.h"

#include <fstream>
#include <string> 
#include <iostream>

namespace core { 
namespace graphics {

	class shader
	{
		public:
			GLuint ShaderID;
			shader(const char *VertexPath, const char *FragmentPath, const char *GeometryPath = 0);

			void Enable();
			void Disable();
	};

}}
