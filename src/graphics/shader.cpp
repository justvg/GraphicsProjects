#include "shader.h"

namespace core {
namespace graphics {

	inline std::string ReadEntireFile(const char *FilePath)
	{
		std::ifstream Ifs(FilePath);
		Ifs.seekg(0, std::ios::end);
		unsigned int Size = Ifs.tellg();
		std::string Result(Size, ' ');
		Ifs.seekg(0, std::ios::beg);
		Ifs.read(&Result[0], Size);

		Ifs.close();
		return(Result);
	}

	shader::shader(const char *VertexPath, const char *FragmentPath)
	{
		GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		ShaderID = glCreateProgram();

		std::string VertexString = ReadEntireFile(VertexPath);
		std::string FragmentString = ReadEntireFile(FragmentPath);

		const char *VertexSource = VertexString.c_str();	
		const char *FragmentSource = FragmentString.c_str();	

		glShaderSource(VertexShader, 1, &VertexSource, 0);
		glCompileShader(VertexShader);

		glShaderSource(FragmentShader, 1, &FragmentSource, 0);
		glCompileShader(FragmentShader);

		glAttachShader(ShaderID, VertexShader);
		glAttachShader(ShaderID, FragmentShader);
		glLinkProgram(ShaderID);

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
	}

	void shader::Enable()
	{
		glUseProgram(ShaderID);
	}

	void shader::Disable()
	{
		glUseProgram(0);
	}

}}