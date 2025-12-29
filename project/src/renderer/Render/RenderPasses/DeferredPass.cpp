#include "DeferredPass.h"
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

DeferredPass::DeferredPass()
{
}

DeferredPass::~DeferredPass()
{
   
}

void DeferredPass::Init()
{
	//m_pShaderModule = ShaderModule::GetShaderModule("D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\gBufferShader.vsh", "D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\gBufferShader.fsh");
	//texture1 = TextureHelper::CreateTextureFromFile("D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_texture.png");
	m_pShaderModule = ShaderModule::GetShaderModule("G:\\JNRenderer\\JNRenderer\\shaders\\gBufferShader.vsh", "G:\\JNRenderer\\JNRenderer\\shaders\\gBufferShader.fsh");
	texture1 = TextureHelper::CreateTextureFromFile("G:\\JNRenderer\\JNRenderer\\models\\spot\\spot_texture.png");
	if (m_gBuffer1)
	{
		m_gBuffer1->Release();
		m_gBuffer1.reset();
	}
	if (m_gBuffer2)
	{
		m_gBuffer2->Release();
		m_gBuffer2.reset();
	}

	if (m_gBuffer3)
	{
		m_gBuffer3->Release();
		m_gBuffer3.reset();
	}



	TextureDesc desc{};
	desc.width = m_uWidth;
	desc.height = m_uHeight;
	desc.format = JNF_RGBA;

	m_gBuffer1 = TextureHelper::CreateTexture(desc);
	m_gBuffer2 = TextureHelper::CreateTexture(desc);
	m_gBuffer3 = TextureHelper::CreateTexture(desc);

}

void DeferredPass::_RenderScene(std::shared_ptr<Scene> scene)
{
	
	//frame = frame > 360 ? 1 : frame + 1.f;s
	
	m_pShaderModule->use();
	const std::vector<std::shared_ptr<Model>>& models = scene->getModels();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Camera* camera= scene->getCamera();
	CameraData cameraData = camera->getCameraData();

	m_pShaderModule->setUniformMatrix4fv("mvpMatrix", cameraData.matViewProject);

	texture1->BindTexture(0);
	m_pShaderModule->useTexture("texture1", 0);

	for(auto model : models)
	{
		model->render();
	}
}

auto DeferredPass::_RecreateRenderResource() -> bool
{
	if (m_uGbufferFBO != -1)
	{
		glDeleteFramebuffers(1,&m_uGbufferFBO);
	}
	

	glGenFramebuffers(1, &m_uGbufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uGbufferFBO);
	
	// - 位置颜色缓冲
	glBindTexture(GL_TEXTURE_2D, m_gBuffer1->GetHandle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_uWidth, m_uHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBuffer1->GetHandle(), 0);

	// - 法线颜色缓冲
	glBindTexture(GL_TEXTURE_2D, m_gBuffer2->GetHandle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_uWidth, m_uHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBuffer2->GetHandle(), 0);

	// - 颜色 + 镜面颜色缓冲
	glBindTexture(GL_TEXTURE_2D, m_gBuffer3->GetHandle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBuffer3->GetHandle(), 0);
	return true;
}

void DeferredPass::OnResize(uint32_t renderWidth, uint32_t renderHeight)
{
	if (m_uWidth != renderWidth || m_uHeight != renderHeight)
	{
		m_uWidth = renderWidth;
		m_uHeight = renderHeight;
		_RecreateRenderResource();
		
	}
}

void DeferredPass::Update()
{
}

void DeferredPass::Render()
{
	RenderContext context = RenderSystem::GetRenderContext();
	//unsigned int fbo = RenderSystem::GetMainFBO();
	glBindFramebuffer(GL_FRAMEBUFFER, m_uGbufferFBO);


		// - 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	_RenderScene(context.scene);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DeferredPass::Release()
{
}

