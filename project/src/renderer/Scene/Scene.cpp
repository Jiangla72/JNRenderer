#include "Scene.h"
#include "Base/Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include "Render/Mesh.h"
#include "Render/Model.h"
#include "Render/Camera.h"
#include "Render/Light.h"
#include "Resource/ResourceManager.h"
Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::init()
{
	m_pCamera = new Camera(45,1,.1f,100);
	m_pCamera->setPos({0,0,1});
}

void Scene::release()
{
}

void Scene::Add(std::shared_ptr<Model> object)
{
  
	models.push_back(object);
}

void Scene::Add(const std::string& filename)
{
    std::shared_ptr<Model> model = std::make_shared<Model>();
    model = ResourceManager::GetResource<Model>(filename);
    models.push_back(model);
}

void Scene::Add(Light* light)
{
    lights.push_back(light);
}

Camera* Scene::getCamera()
{
	return m_pCamera;
}

const std::vector<std::shared_ptr<Model>>& Scene::getModels()
{
	return models;
	// TODO: 在此处插入 return 语句
}

const std::vector<Light*>& Scene::getLights()
{
    return lights;
    // TODO: 在此处插入 return 语句
}

void Scene::update()
{
}

void Scene::OnActive()
{
    m_bActived = true;
}

void Scene::OnDeactive()
{
    m_bActived = false;
}
