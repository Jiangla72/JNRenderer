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