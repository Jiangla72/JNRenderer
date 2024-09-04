#pragma once
#include "ILoader.h"
class ShaderLoader : public ILoader
{
public:
	ShaderLoader ();
	~ShaderLoader();
	virtual bool Load(const std::string& resPath, std::shared_ptr<IResource> pRes);
private:

};

