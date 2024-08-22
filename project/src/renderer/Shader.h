#pragma once
#include "core.h"
#include <glm/glm.hpp>
#include<vector>
#include<glad/glad.h>
#include<gl/GL.h>
#include <memory>
#include <string>
class Triangle;
class Scene;
class Camera;
class JNAPI Shader
{
private:
	std::string vertexFilePath;
	std::string fragmentFilePath;
	unsigned int vertexShader;
	unsigned int fragmentShader;
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
	Shader(const char* vsfilepath, const char* fsfilepath);
	~Shader();
	auto getProgram()
	{
		return shaderProgram;
	}
	void init();
	void CompileShader();
	void use();
	bool setUniform3fv(const std::string& str, const glm::vec3& vec3Value);
	bool setUniformMatrix4fv(const std::string& str, const glm::mat4x4& matValue);
	bool setUniformBuffer(const std::string& str, const void* data, size_t uSize);
private:
	bool _CreateUniformBuffer(const std::string& name,size_t uSize);
};