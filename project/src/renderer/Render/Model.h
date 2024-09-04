#pragma once
#include "Resource/IResource.h"
#include "Resource/ModelLoader.h"
#include <iostream>
#include <glm/glm.hpp>
class Mesh;
class Triangle;
class Model: public IResource
{
public:
	std::vector<Mesh*> m_vecMesh;
public:
	Model();
	~Model();
public:
	virtual void Init();
	virtual void Release();
	virtual const std::string& GetName() const;
	virtual uint32_t GetSize() const;
	static ResourceType GetType();
public:
	void render();
private:
	void _Load(const std::string& obj);
public:
	friend class ModelLoader;
};

