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
#include "Util/GeometryUtils.h"
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::init()
{
	m_pCamera = new Camera(45,1,.1f,100);
	// Move camera back to see the origin (0,0,0)
	m_pCamera->setPos({0, 1, 1}); // Moved up and back
	m_pCamera->setPitchAndYaw(-20, 0.0f); // Look down slightly
}

void Scene::release()
{
    delete m_pCamera;
    m_pCamera = nullptr;
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

void Scene::AddCube(float size, const glm::vec3& pos)
{
    auto mesh = GeometryUtils::CreateCube(size);
    auto model = std::make_shared<Model>();
    model->m_vecMesh.push_back(mesh);
    model->SetPosition(pos);
    models.push_back(model);
}

void Scene::AddSphere(float radius, int sectors, int stacks, const glm::vec3& pos)
{
    auto mesh = GeometryUtils::CreateSphere(radius, sectors, stacks);
    auto model = std::make_shared<Model>();
    model->m_vecMesh.push_back(mesh);
    model->SetPosition(pos);
    models.push_back(model);
}

void Scene::AddPlane(float width, float height, const glm::vec3& pos)
{
    auto mesh = GeometryUtils::CreatePlane(width, height);
    auto model = std::make_shared<Model>();
    model->m_vecMesh.push_back(mesh);
    model->SetPosition(pos);
    models.push_back(model);
}

void Scene::AddCylinder(float radius, float height, int sectors, const glm::vec3& pos)
{
    auto mesh = GeometryUtils::CreateCylinder(radius, height, sectors);
    auto model = std::make_shared<Model>();
    model->m_vecMesh.push_back(mesh);
    model->SetPosition(pos);
    models.push_back(model);
}

Camera* Scene::getCamera()
{
	return m_pCamera;
}

const std::vector<std::shared_ptr<Model>>& Scene::getModels()
{
	return models;
}

const std::vector<Light*>& Scene::getLights()
{
    return lights;
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
