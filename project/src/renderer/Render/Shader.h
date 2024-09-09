#pragma once
#include "Base/core.h"
#include "Resource/IResource.h"
#include <glad/glad.h>
#include <gl/GL.h>
#include <string>
class Triangle;
class Scene;
class Camera;

enum ShaderType
{
	ST_Unknown,
	ST_Vertex,
	ST_Fragment,
	ST_Geometry,
	ST_Hull,
	ST_Domin,
	ST_Compute,
};
class JNAPI Shader :public IResource
{
private:
	unsigned int m_uShaderHandle;
	ShaderType m_eType = ST_Unknown;
public:
	Shader();
	~Shader();

	void init(const char* cfilepath);
	unsigned int GetHandle();
	ShaderType GetShaderType();

public:
	virtual void Init() {};
	virtual void Release();
	virtual const std::string& GetName() const;
	virtual uint32_t GetSize() const;
	static  ResourceType GetType();
private:
	void _CompileShader(const char* cfilepath);

};