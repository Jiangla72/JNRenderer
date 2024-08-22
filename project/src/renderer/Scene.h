#pragma once
#include "core.h"
#include <vector>
#include <string>
class Model;
class Camera;
struct Light;
class JNAPI Scene
{
public:
	int width = 640;
	int height = 640;
private:
	std::vector<Model*> models;
	std::vector<Light*> lights;
	Camera* m_pCamera = nullptr;
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;
public:
	Scene();
	~Scene();
public:
	void init();
	void release();
	void Add(Model* object);
	void Add(const std::string& filename);
	void Add(Light* light);
	Camera* getCamera();
	const std::vector<Model*>& getModels();
	const std::vector<Light*>& getLights();
	void update();

};