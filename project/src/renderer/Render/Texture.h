#pragma once
#include "Base/core.h"
#include <glm/glm.hpp>
#include <string>

enum ColorChanel
{
	CC_RGB,
	CC_RGBA,
};
class JNAPI Texture
{
private:
	int width;
	int height;
	int nrChannels;
	unsigned int texture;

public:
	Texture(uint16_t width, uint16_t height, ColorChanel nrChannels);
	Texture(const std::string& texturePath);
	~Texture();
	int ConvertEnmColorChanelToGLFormat(ColorChanel enmType);
	bool BindTexture(uint16_t uLocation);
	bool UnBindTexture(uint16_t uLocation);
};