#pragma once
#include <string>
#include <memory>
class IResource;
class ILoader
{
public:
	virtual bool Load(const std::string& resPath, std::shared_ptr<IResource> pRes) = 0;

};
