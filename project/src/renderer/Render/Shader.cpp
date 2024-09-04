#include "Shader.h"
#include "Base/Input.h"
#include "Base/LogSystem.h"
#include "Triangle.h"
#include "Camera.h"
#include "Model.h"
#include "Mesh.h"
#include "Scene/Scene.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <algorithm>
#include <fstream>
#include <sstream>
Shader::Shader(const char* vsfilepath, const char* fsfilepath)
	:vertexFilePath(vsfilepath), fragmentFilePath(fsfilepath)
{
}

Shader::~Shader()
{
}

void Shader::init()
{
	int success;
	char infoLog[512];

	CompileShader();
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		JNLOGERROR("ERROR::SHADER::LINK::COMPILATION_FAILED\n{}\n", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::CompileShader()
{
	int  success;
	char infoLog[512];

	std::ifstream vsfin(vertexFilePath);
	std::ifstream fsfin(fragmentFilePath);

	std::stringstream vsbuffer;
	std::stringstream fsbuffer;

	vsbuffer << vsfin.rdbuf();
	fsbuffer << fsfin.rdbuf();

	auto tempVS = vsbuffer.str();
	auto tempFS = fsbuffer.str();
	const char* vertexShaderSource = tempVS.c_str();
	const char* fragmentShaderSource = tempFS.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		JNLOGERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}\n", infoLog);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		JNLOGERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}\n", infoLog);
	}
}

void Shader::use()
{
	glUseProgram(shaderProgram);
}

bool Shader::setUniform3fv(const std::string& str, const glm::vec3& vec3Value)
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

bool Shader::setUniformMatrix4fv(const std::string& str, const glm::mat4x4& matValue)
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

bool Shader::setUniformBuffer(const std::string& str, const void* data, size_t uSize)
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
		JNLOGERROR("shader中找不到 {} UniformBuffer!\n", str);
		return false;
	}
	glUniformBlockBinding(getProgram(), uIndex, bufferData.uBinding);
	glBindBufferBase(GL_UNIFORM_BUFFER, bufferData.uBinding, bufferData.uUboIndex);

	return true;
}

bool Shader::useTexture(const std::string& str, uint16_t uLocation)
{
	glUniform1i(glGetUniformLocation(shaderProgram, str.c_str()), uLocation); // 手动设置
	return true;
}

bool Shader::_CreateUniformBuffer(const std::string& name, size_t uSize)
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

