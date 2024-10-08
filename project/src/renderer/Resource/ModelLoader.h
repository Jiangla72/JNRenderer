#pragma once

#include "Base/core.h"
#include "IResource.h"
#include "ILoader.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

class Model;
class JNAPI ModelLoader : public ILoader
{

public:
	ModelLoader();
	~ModelLoader();
public:
	virtual bool Load(const std::string& resPath, std::shared_ptr<IResource> pRes) override;

private:
	bool _LoadOBJ(const std::string& resPath, std::shared_ptr<Model> pRes);
	bool _LoadFBX(const std::string& resPath, std::shared_ptr<Model> pRes);
	bool _LoadGLTF(const std::string& resPath, std::shared_ptr<Model> pRes);

	friend Model;
};
