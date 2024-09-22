#pragma once
#include "ILoader.h"
class JNAPI TextureLoader : public ILoader
{
public:
	TextureLoader();
	~TextureLoader();
	virtual bool Load(const std::string& resPath, std::shared_ptr<IResource> pRes);
private:

};
