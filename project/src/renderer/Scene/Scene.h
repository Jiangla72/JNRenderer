#pragma once
#include "Base/core.h"
#include <vector>
#include <string>
#include <memory>
class Model;
class Camera;
struct Light;
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
	Camera* getCamera();
	const std::vector<std::shared_ptr<Model>>& getModels();
	const std::vector<Light*>& getLights();
	void update();
	void OnActive();
	void OnDeactive();
};