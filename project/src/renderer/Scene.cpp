#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include "Scene.h"
#include "Mesh.h"
#include"Model.h"
#include"Camera.h"
Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::init()
{
	m_pCamera = new Camera(45,1,30,100);
	m_pCamera->setPos({0,0,1});
}

void Scene::release()
{
	for each (auto model in models)
	{
		delete model;
	}
}

void Scene::Add(Model* object)
{
	models.push_back(object);
}

void Scene::Add(const std::string& filename)
{
	Model* mesh = new Model(filename);
	models.push_back(mesh);
}

Camera* Scene::getCamera()
{
	return m_pCamera;
}

const std::vector<Model*>& Scene::getModels()
{
	return models;
	// TODO: 在此处插入 return 语句
}
