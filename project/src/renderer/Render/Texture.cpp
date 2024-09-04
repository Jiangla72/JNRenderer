#include "Texture.h"
#include "Base/LogSystem.h"
#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>
Texture::Texture(uint16_t width,uint16_t height, ColorChanel enmColorChanel)
	:width(width),height(height)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint format = ConvertEnmColorChanelToGLFormat(enmColorChanel);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::Texture(const std::string& texturePath)
{
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		GLint format = 0;
		if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		else
			JNLOGERROR("加载的纹理通道数不支持！nrChannels: {}", nrChannels);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		JNLOGERROR("Failed to load texture");
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::~Texture()
{
}

int Texture::ConvertEnmColorChanelToGLFormat(ColorChanel enmType)
{
	GLint format = 0;
	switch (enmType)
	{
	case CC_RGB:
		format = GL_RGB;
		break;
	case CC_RGBA:
		format = GL_RGBA;
		break;
	default:
		JNLOGERROR("不支持的ColorChanel类型！");
		break;
	}
	return format;
}

bool Texture::BindTexture(uint16_t uLocation)
{
	
	glActiveTexture(GL_TEXTURE0 + uLocation);
	glBindTexture(GL_TEXTURE_2D, texture);
	return true;
}

bool Texture::UnBindTexture(uint16_t uLocation)
{
	//glActiveTexture(GL_TEXTURE0 + uLocation);
	//glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
