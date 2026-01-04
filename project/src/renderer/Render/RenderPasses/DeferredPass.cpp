#include "DeferredPass.h"
#include "Base/Input.h"
#include "Base/Config.h"
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
	m_uWidth = 640;
	m_uHeight = 480;
}

DeferredPass::~DeferredPass()
{
   
}

void DeferredPass::Init()
{
	//m_pShaderModule = ShaderModule::GetShaderModule("D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\gBufferShader.vsh", "D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\gBufferShader.fsh");
	//texture1 = TextureHelper::CreateTextureFromFile("D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_texture.png");
	m_pShaderModule = ShaderModule::GetShaderModule(Config::GetShaderPath("gBuffer", "vertex").c_str(), Config::GetShaderPath("gBuffer", "fragment").c_str());
	texture1 = TextureHelper::CreateTextureFromFile(Config::GetModelTexturePath("spot"));
	
	// Create default white texture
	//TextureDesc texDesc;
	//texDesc.width = 1;
	//texDesc.height = 1;
	//texDesc.format = JNF_RGBA;
	//texture1 = TextureHelper::CreateTexture(texDesc);
	
	// Fill with white
	glBindTexture(GL_TEXTURE_2D, texture1->GetHandle());
	unsigned char white[] = { 255, 255, 255, 255 };
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, white);
	glBindTexture(GL_TEXTURE_2D, 0);

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

	if (m_gBuffer4)
	{
		m_gBuffer4->Release();
		m_gBuffer4.reset();
	}

	TextureDesc desc{};
	desc.width = m_uWidth;
	desc.height = m_uHeight;
	desc.format = JNF_RGBA;

	m_gBuffer1 = TextureHelper::CreateTexture(desc);
	m_gBuffer2 = TextureHelper::CreateTexture(desc);
	m_gBuffer3 = TextureHelper::CreateTexture(desc);
	m_gBuffer4 = TextureHelper::CreateTexture(desc);

}

void DeferredPass::_RenderScene(std::shared_ptr<Scene> scene)
{
	
	//frame = frame > 360 ? 1 : frame + 1.f;s
	
	m_pShaderModule->use();
	const std::vector<std::shared_ptr<Model>>& models = scene->getModels();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Camera* camera= scene->getCamera();
	CameraData cameraData = camera->getCameraData();

	texture1->BindTexture(0);
	m_pShaderModule->useTexture("texture1", 0);

	for(auto& model : models)
	{
		glm::mat4 modelMatrix = model->GetModelMatrix();
		glm::mat4 mvpMatrix = cameraData.matViewProject * modelMatrix;
		
		m_pShaderModule->setUniformMatrix4fv("mvpMatrix", mvpMatrix);
		m_pShaderModule->setUniformMatrix4fv("model", modelMatrix);
		
		model->render();
	}
}

bool DeferredPass::_RecreateRenderResource()
{
	if (m_uGbufferFBO != -1)
	{
		glDeleteFramebuffers(1,&m_uGbufferFBO);
	}
	
	if (m_depthRBO != -1)
	{
		glDeleteRenderbuffers(1, &m_depthRBO);
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

	// - 颜色 + 镜面反射颜色缓冲
	glBindTexture(GL_TEXTURE_2D, m_gBuffer3->GetHandle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBuffer3->GetHandle(), 0);

	// - 金属度 + 粗糙度缓冲
	glBindTexture(GL_TEXTURE_2D, m_gBuffer4->GetHandle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gBuffer4->GetHandle(), 0);

	// - 深度缓冲
	glGenRenderbuffers(1, &m_depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_uWidth, m_uHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//std::cout << "Framebuffer not complete!" << std::endl;
		return false;

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
	RenderContext& context = RenderSystem::GetRenderContext();
	//unsigned int fbo = RenderSystem::GetMainFBO();
	glBindFramebuffer(GL_FRAMEBUFFER, m_uGbufferFBO);
	// 确保与G-Buffer尺寸一致的视口，避免上一Pass修改导致尺寸不匹配
	glViewport(0, 0, m_uWidth, m_uHeight);

	// 更新RenderContext中的G-Buffer数据
	context.m_gBuffer1 = m_gBuffer1;
	context.m_gBuffer2 = m_gBuffer2;
	context.m_gBuffer3 = m_gBuffer3;
	context.m_gBuffer4 = m_gBuffer4;

	// - 告诉OpenGL需要使用(帧缓冲)多个颜色附件进行渲染
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);
	_RenderScene(context.scene);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DeferredPass::Release()
{
	if (m_uGbufferFBO != -1)
	{
		glDeleteFramebuffers(1, &m_uGbufferFBO);
	}
	if (m_depthRBO != -1)
	{
		glDeleteRenderbuffers(1, &m_depthRBO);
	}
}

