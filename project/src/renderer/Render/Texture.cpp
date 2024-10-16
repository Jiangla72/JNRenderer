#include "Texture.h"
#include "Base/LogSystem.h"
#include "Resource/ResourceManager.h"
#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>
Texture::Texture(uint16_t width,uint16_t height, JNFormat enmColorChanel)
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
	Init(texturePath);
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Init(const std::string& cPath)
{
	unsigned char* data = stbi_load(cPath.c_str(), &width, &height, &nrChannels, 0);
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

int Texture::ConvertEnmColorChanelToGLFormat(JNFormat enmType)
{
	GLint format = 0;
	switch (enmType)
	{
	case JNF_RGB:
		format = GL_RGB;
		break;
	case JNF_RGBA:
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

void Texture::Init()
{
}

void Texture::Release()
{
}

const std::string& Texture::GetName() const
{
	// TODO: 在此处插入 return 语句
	return "";
}

uint32_t Texture::GetSize() const
{
	return 0;
}

auto Texture::Resize(uint32_t i_width, uint32_t i_height) -> bool
{
	if (width != i_width || height != i_height)
	{

		glBindTexture(GL_TEXTURE_2D, texture);
		width = i_width;
		height = i_height;
		unsigned int format = 0;
		if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return true;
}

namespace TextureHelper {

	std::shared_ptr<Texture> CreateTexture(const TextureDesc& sDesc)
	{
		std::shared_ptr<Texture> pTexture = std::make_shared<Texture>(sDesc.width, sDesc.height, sDesc.format);
		return pTexture;
	}

	std::shared_ptr<Texture> CreateTextureFromFile(const std::string& strFile)
	{
		std::shared_ptr<Texture> pTexture = ResourceManager::GetResource<Texture>(strFile);
		return pTexture;
	}

};
