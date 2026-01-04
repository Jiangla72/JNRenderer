#pragma once
#include "Base/core.h"
#include <vector>
#include <string>
#include <memory>
class Model;
class Camera;
#include "Render/Light.h"
class JNAPI Scene
{
public:
	int width = 640;
	int height = 640;
	bool m_bActived = false;
private:
	std::vector<std::shared_ptr<Model>> models;
	std::vector<Light*> lights;
	Camera* m_pCamera = nullptr;
public:
	Scene();
	~Scene();
public:
	void init();
	void release();
	void Add(std::shared_ptr<Model> object);
	void Add(const std::string& filename);
	void Add(Light* light);
	
	// Primitive creation helpers
	void AddCube(float size = 1.0f, const glm::vec3& pos = glm::vec3(0.0f));
	void AddSphere(float radius = 0.5f, int sectors = 32, int stacks = 16, const glm::vec3& pos = glm::vec3(0.0f));
	void AddPlane(float width = 10.0f, float height = 10.0f, const glm::vec3& pos = glm::vec3(0.0f));
	void AddCylinder(float radius = 0.5f, float height = 1.0f, int sectors = 32, const glm::vec3& pos = glm::vec3(0.0f));

	Camera* getCamera();
	const std::vector<std::shared_ptr<Model>>& getModels();
	const std::vector<Light*>& getLights();
	void update();
	void OnActive();
	void OnDeactive();
};
