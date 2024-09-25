#include "RenderSystem.h"
#include "RenderPasses/IPass.h"
#include "RenderPasses/DefaltForwardPass.h"
#include "Render/Texture.h"
#include "Scene/SceneManager.h"
#include "engine.h"
#include "Scene/Scene.h"
#include "Camera.h"
RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{
}

unsigned int RenderSystem::GetMainFBO()
{
	return Engine::GetEngine()->GetSystem<RenderSystem>()->fbo;
}

void RenderSystem::OnInit()
{


	DefaltForwardPass* pass = new DefaltForwardPass();
	m_vecPasses.push_back(pass);
	for (auto pass : m_vecPasses)
	{
		pass->Init();
	}

}

void RenderSystem::OnUninit()
{
	for (auto pass : m_vecPasses)
	{
		pass->Release();
	}

}

void RenderSystem::OnUpdate()
{
	for (auto pass : m_vecPasses)
	{
		pass->Update();
	}
}

void RenderSystem::OnRender()
{
	_UpdateRenderContext();
	for (auto pass:m_vecPasses)
	{
		pass->Render();
	}
}

void RenderSystem::_UpdateRenderContext()
{
	m_sRenderContext.scene = SceneManager::GetActiveScene();

}

RenderContext& RenderSystem::GetRenderContext()
{
	return 	Engine::GetEngine()->GetSystem<RenderSystem>()->m_sRenderContext;
}

void RenderSystem::SetRenderTarget(std::shared_ptr<Texture> texture, int width, int height)
{
	m_pTexture = texture;
	m_nWidth = width;
	m_nHeight = height;
	// 1、创建FBO对象
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// 2、创建颜色缓冲区对象并附加到FBO
	glGenRenderbuffers(1, &colorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);

	// 3、创建深度缓冲区对象并附加到FBO
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetHandle(), 0);

	// 检查FBO是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		// 处理错误
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}


void RenderSystem::ResizeColorAttachment(uint32_t width, uint32_t height)
{
	if (fbo != -1 && (width != m_nWidth || height != m_nHeight))
	{
		m_nWidth = width;
		m_nHeight = height;
		glBindTexture(GL_TEXTURE_2D, m_pTexture->GetHandle());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// 2、创建颜色缓冲区对象并附加到FBO
		glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);

		// 3、创建深度缓冲区对象并附加到FBO
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetHandle(), 0);

		// 检查FBO是否完整
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			// 处理错误
		}

		glViewport(0, 0, width, height);

		Camera* pMainCamera = m_sRenderContext.scene->getCamera();
		pMainCamera->setAspectRatio((float)width / (float)height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}

