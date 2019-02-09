#pragma once

#include "GL\glew.h"

#include <fstream>
#include <string> 

namespace core { 
namespace graphics {

	class shader
	{
		public:
			GLuint ShaderID;
			shader(const char *VertexPath, const char *FragmentPath);

			void Enable();
			void Disable();
	};

}}
