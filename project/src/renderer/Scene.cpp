#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include "Scene.h"
#include "Mesh.h"
#include"Model.h"
#include"Camera.h"
#include"Input.h"
#include "Light.h"
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

void Scene::Add(Light* light)
{
    lights.push_back(light);
}

Camera* Scene::getCamera()
{
	return m_pCamera;
}

const std::vector<Model*>& Scene::getModels()
{
	return models;
	// TODO: �ڴ˴����� return ���
}

const std::vector<Light*>& Scene::getLights()
{
    return lights;
    // TODO: �ڴ˴����� return ���
}

void Scene::update()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrameTime;
    lastFrameTime = currentFrame;

    glm::vec3 moveDir = { 0.0f,0.0f,0.0f };
    float cameraSpeed = 0.2f * deltaTime;
    Camera* l_Camera = getCamera();
    CameraData cameraData = l_Camera->getCameraData();
    glm::vec3 cameraPos = cameraData.pos;
    if (Input::getKey(GLFW_KEY_UP) || Input::getKey(GLFW_KEY_W))
    {
        moveDir += cameraData.forward * (1.0f);
    }
    if (Input::getKey(GLFW_KEY_LEFT) || Input::getKey(GLFW_KEY_A))
    {
        moveDir += glm::normalize(glm::cross(cameraData.forward, cameraData.up)) * (-1.0f);
    }
    if (Input::getKey(GLFW_KEY_DOWN) || Input::getKey(GLFW_KEY_S))
    {
        moveDir += cameraData.forward * (-1.0f);
    }
    if (Input::getKey(GLFW_KEY_RIGHT) || Input::getKey(GLFW_KEY_D))
    {
        moveDir += glm::normalize(glm::cross(cameraData.forward, cameraData.up)) * (1.0f);
    }
    if (Input::getKey(GLFW_KEY_Q))
    {
        moveDir += cameraData.up * (1.0f);
    }
    if (Input::getKey(GLFW_KEY_E))
    {
        moveDir += cameraData.up * (-1.0f);
    }
    cameraPos += cameraSpeed * moveDir;
    l_Camera->setPos(cameraPos);

    if (Input::mouseMoving && Input::getMouseButton(1))
    {
        float xoffset = Input::mouseMotion.x;
        float yoffset = -Input::mouseMotion.y;
        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        cameraData.pitch += yoffset;
        cameraData.yaw += xoffset;
        l_Camera->setPitchAndYaw(cameraData.pitch + yoffset, cameraData.yaw + xoffset);
        Input::mouseMoving = false;
    }
}