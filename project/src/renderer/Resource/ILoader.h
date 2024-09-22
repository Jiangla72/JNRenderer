#pragma once
#include "Base/core.h"

#include <string>
#include <memory>
class IResource;
class JNAPI ILoader
{
public:
	virtual bool Load(const std::string& resPath, std::shared_ptr<IResource> pRes) = 0;

};
