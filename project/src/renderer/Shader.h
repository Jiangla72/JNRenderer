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
};