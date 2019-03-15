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

	shader::shader(const char *VertexPath, const char *FragmentPath, const char *GeometryPath)
	{
		GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLuint GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		ShaderID = glCreateProgram();

		std::string VertexString = ReadEntireFile(VertexPath);
		std::string FragmentString = ReadEntireFile(FragmentPath);

		const char *VertexSource = VertexString.c_str();	
		const char *FragmentSource = FragmentString.c_str();	

		glShaderSource(VertexShader, 1, &VertexSource, 0);
		glCompileShader(VertexShader);
		int Success;
		char InfoLog[1024];
		glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(VertexShader, 1024, NULL, InfoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "VertexShader" << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

		glShaderSource(FragmentShader, 1, &FragmentSource, 0);
		glCompileShader(FragmentShader);
		glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(FragmentShader, 1024, NULL, InfoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "FragmentShader" << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

		if(GeometryPath)
		{
			std::string GeometryString = ReadEntireFile(GeometryPath);
			const char *GeometrySource = GeometryString.c_str();

			glShaderSource(GeometryShader, 1, &GeometrySource, 0);
			glCompileShader(GeometryShader);
			glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				glGetShaderInfoLog(GeometryShader, 1024, NULL, InfoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "GeometryShader" << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}

		glAttachShader(ShaderID, VertexShader);
		glAttachShader(ShaderID, FragmentShader);
		if(GeometryPath)
		{
			glAttachShader(ShaderID, GeometryShader);
		}
		glLinkProgram(ShaderID);
		glGetProgramiv(ShaderID, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(ShaderID, 1024, NULL, InfoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "Program" << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		glDeleteShader(GeometryShader);
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