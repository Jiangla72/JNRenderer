#pragma once
#include "Base/core.h"

#include "ILoader.h"
class JNAPI ShaderLoader : public ILoader
{
public:
	ShaderLoader ();
	~ShaderLoader();
	virtual bool Load(const std::string& resPath, std::shared_ptr<IResource> pRes) override;
private:

};

