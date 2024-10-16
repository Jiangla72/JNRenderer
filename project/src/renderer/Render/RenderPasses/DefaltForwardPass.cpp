#include "DefaltForwardPass.h"
#include "Base/Input.h"
#include "Render/RenderSystem.h"
#include "Render/Triangle.h"
#include "Render/Camera.h"
#include "Render/Model.h"
#include "Render/Mesh.h"
#include "Render/ShaderModule.h"
#include "Render/Light.h"
#include "Render/Texture.h"
#include "Scene/Scene.h"
#include <array>
#include <algorithm>
#include <GLFW/glfw3.h>

DefaltForwardPass::DefaltForwardPass()
{
}

DefaltForwardPass::~DefaltForwardPass()
{
   
}

void DefaltForwardPass::Init()
{
	m_pShaderModule = ShaderModule::GetShaderModule("D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\defaultShader.vsh", "D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\defaultShader.fsh");
	texture1 = TextureHelper::CreateTextureFromFile("D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_texture.png");

}

void DefaltForwardPass::_RenderScene(std::shared_ptr<Scene> scene)
{
	
	//frame = frame > 360 ? 1 : frame + 1.f;s
	
	m_pShaderModule->use();
	 const std::vector<std::shared_ptr<Model>>& models = scene->getModels();
	 const std::vector<Light*>& lights = scene->getLights();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Camera* camera= scene->getCamera();
	CameraData cameraData = camera->getCameraData();

	m_pShaderModule->setUniformMatrix4fv("mvpMatrix", cameraData.matViewProject);
	m_pShaderModule->setUniform3fv("cameraPos", cameraData.pos);

	texture1->BindTexture(17);
	m_pShaderModule->useTexture("texture1", 17);
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
	m_pShaderModule->setUniformBuffer("LightUBO", (void*)&lightInfo, sizeof(LightInfoForGPU));
	for(auto model : models)
	{
		model->render();
	}
}

void DefaltForwardPass::OnResize(uint32_t renderWidth, uint32_t renderHeight)
{
}

void DefaltForwardPass::Update()
{
}

void DefaltForwardPass::Render()
{
	RenderContext context = RenderSystem::GetRenderContext();
	unsigned int fbo = RenderSystem::GetMainFBO();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	_RenderScene(context.scene);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DefaltForwardPass::Release()
{
}

