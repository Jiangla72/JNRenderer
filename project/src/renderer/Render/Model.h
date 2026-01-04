#pragma once
#include "Resource/IResource.h"
#include "Resource/ModelLoader.h"
#include <iostream>
#include <glm/glm.hpp>
class Mesh;
class Triangle;
class JNAPI Model: public IResource
{
public:
	std::vector<std::shared_ptr<Mesh>> m_vecMesh;
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
	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::vec3& rot);
	void SetScale(const glm::vec3& scale);
	glm::mat4 GetModelMatrix() const;

private:
	void _Load(const std::string& obj);

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);
	bool m_bDirty = true;
	void UpdateModelMatrix();
public:
	friend class ModelLoader;
};

