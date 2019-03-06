#pragma once

#include <iostream>
#include <string>

namespace core {

	GLuint LoadTexture(const char *Path)
	{
		GLuint TextureID;
		glGenTextures(1, &TextureID);

		int Width, Height, nrChannels;
		unsigned char *Data = stbi_load(Path, &Width, &Height, &nrChannels, 0);
		if(Data)
		{
			GLenum Format;
			if(nrChannels == 1) Format = GL_RED;
			else if(nrChannels == 3) Format = GL_RGB;
			else if(nrChannels == 4) Format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cout << "CAN'T LOAD A TEXTURE" << std::endl;
		}
		stbi_image_free(Data);

		return (TextureID);
	}

	GLuint LoadCubemap(std::vector<std::string> FacePaths)
	{
		GLuint TextureID;
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

		int Width, Height, nrChannels;
		for(uint32_t I = 0; I < FacePaths.size(); I++)
		{
			unsigned char *Data = stbi_load(FacePaths[I].c_str(), &Width, &Height, &nrChannels, 0);
			if(Data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + I, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
			}
			else
			{
				std::cout << "CAN'T LOAD CUBEMAP TEXTURE" << std::endl;
			}
			stbi_image_free(Data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return (TextureID);
	}

}