#include "ShaderModule.h"
#include "Base/LogSystem.h"
#include "Resource/ResourceManager.h"
#include <iostream>
#include "glm/ext.hpp"

#define VS_HASH 16
#define FS_HASH 32
#define GS_HASH 48
#define HS_HASH 56
#define DS_HASH 60

std::unordered_map<size_t, std::shared_ptr<ShaderModule>> ShaderModule::cacheShader;
ShaderModule::ShaderModule(const char* vsfilepath, const char* fsfilepath, const char* gsfilepath,
	const char* hsfilepath,
	const char* dsfilepath)
{
	std::shared_ptr<Shader> pVShader = ResourceManager::GetResource<Shader>(vsfilepath);
	std::shared_ptr<Shader> pFShader = ResourceManager::GetResource<Shader>(fsfilepath);
	std::shared_ptr<Shader> pGShader = ResourceManager::GetResource<Shader>(gsfilepath);
	std::shared_ptr<Shader> pHShader = ResourceManager::GetResource<Shader>(hsfilepath);
	std::shared_ptr<Shader> pDShader = ResourceManager::GetResource<Shader>(dsfilepath);
	_Link(pVShader, pFShader, pGShader, pHShader, pDShader);
}

ShaderModule::ShaderModule(std::shared_ptr<Shader> pVShader, std::shared_ptr<Shader> pFShader, std::shared_ptr<Shader> pGShader,
	std::shared_ptr<Shader> pHShader,
	std::shared_ptr<Shader> pDShader)
{
	_Link(pVShader, pFShader, pGShader, pHShader, pDShader);
}

ShaderModule::~ShaderModule()
{
}

std::shared_ptr<ShaderModule> ShaderModule::GetShaderModule(const char* vsfilepath, const char* fsfilepath, const char* gsfilepath, const char* hsfilepath, const char* dsfilepath)
{
	std::shared_ptr<Shader> pVShader = ResourceManager::GetResource<Shader>(vsfilepath);
	std::shared_ptr<Shader> pFShader = ResourceManager::GetResource<Shader>(fsfilepath);
	std::shared_ptr<Shader> pGShader = ResourceManager::GetResource<Shader>(gsfilepath);
	std::shared_ptr<Shader> pHShader = ResourceManager::GetResource<Shader>(hsfilepath);
	std::shared_ptr<Shader> pDShader = ResourceManager::GetResource<Shader>(dsfilepath);
	return GetShaderModule(pVShader, pFShader, pGShader, pHShader, pDShader);
}

std::shared_ptr<ShaderModule> ShaderModule::GetShaderModule(std::shared_ptr<Shader> pVShader, std::shared_ptr<Shader> pFShader, std::shared_ptr<Shader> pGShader, std::shared_ptr<Shader> pHShader, std::shared_ptr<Shader> pDShader)
{
	size_t hash = 0;
	if (pVShader)
	{
		unsigned int uVS = pVShader->GetHandle();
		hash |= (uVS + 1) << VS_HASH;
	}
	if (pFShader)
	{
		unsigned int uFS = pFShader->GetHandle();
		hash |= (uFS + 1) << FS_HASH;
	}
	if (pGShader)
	{
		unsigned int uGS = pGShader->GetHandle();
		hash |= (uGS + 1) << GS_HASH;
	}
	if (pHShader)
	{
		unsigned int uHS = pHShader->GetHandle();
		hash |= (uHS + 1) << HS_HASH;
	}
	if (pDShader)
	{
		unsigned int uDS = pDShader->GetHandle();
		hash |= (uDS + 1) << DS_HASH;
	}
	std::shared_ptr<ShaderModule> pShaderModule = nullptr;

	if (cacheShader.find(hash) == cacheShader.end())
	{
		pShaderModule = std::make_shared<ShaderModule>(pVShader, pFShader, pGShader, pHShader, pDShader);
		cacheShader.emplace(hash, pShaderModule);
	}
	else
	{
		pShaderModule = cacheShader[hash];
	}
	return pShaderModule;
}

void ShaderModule::_Link(std::shared_ptr<Shader> pVShader, std::shared_ptr<Shader> pFShader,
	std::shared_ptr<Shader> pGShader,
	std::shared_ptr<Shader> pHShader,
	std::shared_ptr<Shader> pDShader)
{
	int success;
	char infoLog[512];
	shaderProgram = glCreateProgram();

	if (pVShader)
	{
		unsigned int vShader = pVShader->GetHandle();
		glAttachShader(shaderProgram, vShader);
	}
	if (pFShader)
	{
		unsigned int fShader = pFShader->GetHandle();
		glAttachShader(shaderProgram, fShader);
	}
	if (pGShader)
	{
		unsigned int gShader = pGShader->GetHandle();
		glAttachShader(shaderProgram, gShader);
	}
	if (pHShader)
	{
		unsigned int hShader = pHShader->GetHandle();
		glAttachShader(shaderProgram, hShader);
	}
	if (pDShader)
	{
		unsigned int dShader = pDShader->GetHandle();
		glAttachShader(shaderProgram, dShader);
	}
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		JNLOGERROR("ERROR::SHADER::LINK::COMPILATION_FAILED\n{}\n", infoLog);
	}
}

void ShaderModule::use()
{
	glUseProgram(shaderProgram);
}

bool ShaderModule::setUniform1i(const std::string& str, int iValue)
{
	int nIndex = glGetUniformLocation(getProgram(), str.c_str());
	if (nIndex != -1)
	{
		glUniform1i(nIndex, iValue);
		return true;
	}
	JNLOGERROR("找不到 {} Uniform1i!\n", str);
	return false;
}

bool ShaderModule::setUniform3fv(const std::string& str, const glm::vec3& vec3Value)
{
	int nIndex = glGetUniformLocation(getProgram(), str.c_str());
	if (nIndex == -1)
	{
		JNLOGERROR("找不到 {} Uniform3fv!\n", str);
		return false;
	}

	glUniform3fv(nIndex, 1, glm::value_ptr(vec3Value));
	return true;
}

bool ShaderModule::setUniformMatrix4fv(const std::string& str, const glm::mat4x4& matValue)
{
	int nIndex = glGetUniformLocation(getProgram(), str.c_str());
	if (nIndex == -1)
	{
		JNLOGERROR("找不到 {} UniformMatrix4fv!\n", str);
		return false;
	}
	glUniformMatrix4fv(nIndex, 1, GL_FALSE, glm::value_ptr(matValue));
	return true;
}

bool ShaderModule::setUniformBuffer(const std::string& str, const void* data, size_t uSize)
{
	UniformBufferData bufferData;
	auto it = mapStrtoUbo.find(str);
	if (it == mapStrtoUbo.end())
	{
		if (!_CreateUniformBuffer(str, uSize))
		{
			JNLOGERROR("找不到 {} UniformBuffer!\n", str);
			return false;
		}
	}
	bufferData = mapStrtoUbo[str];

	glBindBuffer(GL_UNIFORM_BUFFER, bufferData.uUboIndex);
	glBufferData(GL_UNIFORM_BUFFER, uSize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	GLuint uIndex = glGetUniformBlockIndex(getProgram(), str.c_str());
	if (uIndex == GL_INVALID_INDEX)
	{
		JNLOGERROR("ShaderModule中找不到 {} UniformBuffer!\n", str);
		return false;
	}
	glUniformBlockBinding(getProgram(), uIndex, bufferData.uBinding);
	glBindBufferBase(GL_UNIFORM_BUFFER, bufferData.uBinding, bufferData.uUboIndex);

	return true;
}

bool ShaderModule::useTexture(const std::string& str, uint16_t uLocation)
{
	glUniform1i(glGetUniformLocation(shaderProgram, str.c_str()), uLocation); // 手动设置
	return true;
}

bool ShaderModule::_CreateUniformBuffer(const std::string& name, size_t uSize)
{
	auto it = mapStrtoUbo.find(name);
	if (it != mapStrtoUbo.end())
	{
		JNLOGERROR("{} UnifomrBuffer已存在!\n", name);
		return false;
	}
	UniformBufferData uniformBuffer;
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, uSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	uniformBuffer.uBinding = nBinding;
	nBinding++;

	uniformBuffer.uUboIndex = ubo;
	uniformBuffer.uSize = uSize;
	mapStrtoUbo.emplace(name, uniformBuffer);
	//mapStrtoUbo.insert({ name, uniformBuffer });
	//mapStrtoUbo.insert(std::pair<std::string, UniformBufferData>(name, uniformBuffer));
	return true;
}

