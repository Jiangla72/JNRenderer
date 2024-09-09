#include "Shader.h"
#include "Base/Input.h"
#include "Util/StringUtil.h"
#include "Base/LogSystem.h"
#include "Scene/Scene.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::init(const char* cfilepath)
{
	std::string ext = StringUtil::GetFileExt(cfilepath);
	if (ext.compare("vsh") == 0)
	{
		m_eType = ST_Vertex;
	}
	else if (ext.compare("fsh") == 0)
	{
		m_eType = ST_Fragment;
	}
	else if (ext.compare("gsh") == 0)
	{
		m_eType = ST_Geometry;
	}
	else if (ext.compare("hsh") == 0)
	{
		m_eType = ST_Hull;
	}
	else if (ext.compare("dsh") == 0)
	{
		m_eType = ST_Domin;
	}
	else if (ext.compare("csh") == 0)
	{
		m_eType = ST_Compute;
	}
	else
	{
		JNLOGERROR("ERROR::UNKNOWN SHADER TYPE::WITH EXT:{}\n", ext);
	}

	_CompileShader(cfilepath);
}

void Shader::_CompileShader(const char* cfilepath)
{
	int  success;
	char infoLog[512];

	std::ifstream fin(cfilepath);

	std::stringstream buffer;

	buffer << fin.rdbuf();

	auto tempS = buffer.str();
	const char* ShaderSource = tempS.c_str();
	GLuint uShaderType = 0;
	switch (m_eType)
	{
	case ST_Vertex:
		uShaderType = GL_VERTEX_SHADER;
		break;
	case ST_Fragment:
		uShaderType = GL_FRAGMENT_SHADER;
		break;
	case ST_Geometry:
		uShaderType = GL_GEOMETRY_SHADER;
		break;
	case ST_Hull:
		uShaderType = GL_TESS_CONTROL_SHADER;
		break;
	case ST_Domin:
		uShaderType = GL_TESS_EVALUATION_SHADER;
		break;
	case ST_Compute:
		uShaderType = GL_COMPUTE_SHADER;
		break;
	default:
		JNLOGERROR("ERROR::UNKNOWN SHADER TYPE\n");
		break;
	}
	m_uShaderHandle = glCreateShader(uShaderType);
	glShaderSource(m_uShaderHandle, 1, &ShaderSource, NULL);
	glCompileShader(m_uShaderHandle);
	glGetShaderiv(m_uShaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_uShaderHandle, 512, NULL, infoLog);
		JNLOGERROR("ERROR::SHADER::TYPE:{}::COMPILATION_FAILED\n{}\n", uShaderType, infoLog);
	}
}

unsigned int Shader::GetHandle()
{
	return m_uShaderHandle;
}


ShaderType Shader::GetShaderType()
{
	return m_eType;
}

void Shader::Release()
{
}

const std::string& Shader::GetName() const
{
	return "";
}

uint32_t Shader::GetSize() const
{
	return 0;
}

ResourceType Shader::GetType()
{
	return ResourceType::eShader;
}

