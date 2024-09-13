#include "TextureLoader.h"
#include "Render/Texture.h"
TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
}

bool TextureLoader::Load(const std::string& resPath, std::shared_ptr<IResource> pRes)
{
	std::shared_ptr<Texture> texture = std::dynamic_pointer_cast<Texture>(pRes);
	if (texture == nullptr)
	{
		return false;
	}
	texture->Init(resPath);
	return false;
}
