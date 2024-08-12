#include "Shader.h"
#include"Triangle.h"
#include"Scene.h"
#include"Camera.h"
#include"Model.h"
#include "Mesh.h"
#include<array>
#include <algorithm>
#include "Input.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include<sstream>
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
		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << infoLog << std::endl;
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
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::use()
{
	glUseProgram(shaderProgram);
}

