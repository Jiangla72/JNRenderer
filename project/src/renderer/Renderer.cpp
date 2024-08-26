#include "Renderer.h"
#include"Triangle.h"
#include"Scene.h"
#include"Camera.h"
#include"Model.h"
#include "Mesh.h"
#include<array>
#include <algorithm>
#include "Input.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Light.h"
#include "Texture.h"
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
   
}

void Renderer::init()
{
	m_pShader = new Shader("D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\defaultShader.vsh", "D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\defaultShader.fsh");
	m_pShader->init();
	texture1 = new Texture("D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_texture.png");

}

void Renderer::render(std::shared_ptr<Scene> scene)
{
	
	//frame = frame > 360 ? 1 : frame + 1.f;s
	
	 m_pShader->use();
	 const std::vector<Model*>& models = scene->getModels();
	 const std::vector<Light*>& lights = scene->getLights();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Camera* camera= scene->getCamera();
	CameraData cameraData = camera->getCameraData();

	m_pShader->setUniformMatrix4fv("mvpMatrix", cameraData.matViewProject);
	m_pShader->setUniform3fv("cameraPos", cameraData.pos);

	texture1->BindTexture(17);
	m_pShader->useTexture("texture1", 17);
	struct LightInfoForGPU
	{
		Light light[8];
		int nCount = 0;
		glm::ivec3 padding = glm::ivec3(0);
	} lightInfo;
	for (auto light : lights)
	{
		lightInfo.light[(int)lightInfo.nCount] = *light;
		lightInfo.nCount++;
		if (lightInfo.nCount > 7)
			break;
	}
	m_pShader->setUniformBuffer("LightUBO", (void*)&lightInfo, sizeof(LightInfoForGPU));
	for(auto model : models)
	{
		model->render();
	}
}

