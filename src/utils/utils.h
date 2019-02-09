#pragma once

#include <iostream>

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

}