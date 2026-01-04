#pragma once
#include "Base/core.h"
#include "Shader.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <gl/GL.h>
#include <string>
#include <memory>

class JNAPI ShaderModule
{
private:
	unsigned int shaderProgram;
	int nBinding = 0;
	struct UniformBufferData
	{
		uint32_t uUboIndex = -1;
		uint32_t uBinding = -1;
		size_t uSize = 0;
	};
	std::unordered_map<std::string, UniformBufferData> mapStrtoUbo;

public:
	ShaderModule(const char* vsfilepath, const char* fsfilepath,
		const char* gsfilepath = nullptr,
		const char* hsfilepath = nullptr,
		const char* dsfilepath = nullptr);
	ShaderModule(std::shared_ptr<Shader> pVShader, std::shared_ptr<Shader> m_pFShader, 
		std::shared_ptr<Shader> m_pGShader = nullptr, 
		std::shared_ptr<Shader> m_pHShader = nullptr, 
		std::shared_ptr<Shader> m_pDShader = nullptr);
	static std::unordered_map<size_t, std::shared_ptr<ShaderModule>> cacheShader;
	~ShaderModule();

	static std::shared_ptr<ShaderModule> GetShaderModule(const char* vsfilepath, const char* fsfilepath,
		const char* gsfilepath = "",
		const char* hsfilepath = "",
		const char* dsfilepath = "");
	static std::shared_ptr<ShaderModule> GetShaderModule(std::shared_ptr<Shader> pVShader, std::shared_ptr<Shader> m_pFShader,
		std::shared_ptr<Shader> m_pGShader = nullptr,
		std::shared_ptr<Shader> m_pHShader = nullptr,
		std::shared_ptr<Shader> m_pDShader = nullptr);
public:
	auto getProgram()
	{
		return shaderProgram;
	}
	void _Link(std::shared_ptr<Shader> pVShader, std::shared_ptr<Shader> m_pFShader,
		std::shared_ptr<Shader> m_pGShader,
		std::shared_ptr<Shader> m_pHShader,
		std::shared_ptr<Shader> m_pDShader);
	void use();
	bool setUniform1i(const std::string& str, int iValue);
	bool setUniform3fv(const std::string& str, const glm::vec3& vec3Value);
	bool setUniformMatrix4fv(const std::string& str, const glm::mat4x4& matValue);
	bool setUniformBuffer(const std::string& str, const void* data, size_t uSize);
	bool useTexture(const std::string& str, uint16_t uLocation);
private:
	bool _CreateUniformBuffer(const std::string& name,size_t uSize);
};