#include "ShaderLoader.h"
#include "Render/Shader.h"
ShaderLoader::ShaderLoader()
{
}

ShaderLoader::~ShaderLoader()
{
}

bool ShaderLoader::Load(const std::string& resPath, std::shared_ptr<IResource> pRes)
{
	std::shared_ptr<Shader> shader = std::dynamic_pointer_cast<Shader>(pRes);
	if (shader == nullptr)
	{
		return false;
	}
	shader->init(resPath.c_str());
	return true;
}
