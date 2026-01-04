/**
 * @file RenderSystem.cpp
 * @brief 渲染系统实现，管理渲染管道和Pass。
 */

#include "RenderSystem.h"
#include "RenderPasses/IPass.h"
#include "RenderPasses/ShadowPass.h"
#include "RenderPasses/DefaltForwardPass.h"
#include "RenderPasses/DeferredPass.h"
#include "RenderPasses/LightingPass.h"
#include "RenderPasses/PostProcessPass.h"
#include "OpenGLRenderer.h"

#include <algorithm>

#include "Render/Texture.h"
#include "Render/Light.h"
#include "Scene/SceneManager.h"
#include "engine.h"
#include "Scene/Scene.h"
#include "Camera.h"

/**
 * @brief 构造函数
 */
RenderSystem::RenderSystem()
{
}

/**
 * @brief 析构函数
 */
RenderSystem::~RenderSystem()
{
}

/**
 * @brief 获取主FBO
 * @return 主帧缓冲对象ID
 */
unsigned int RenderSystem::GetMainFBO()
{
    return Engine::GetEngine()->GetSystem<RenderSystem>()->fbo;
}

/**
 * @brief 初始化渲染系统
 */
void RenderSystem::OnInit()
{
    m_pRenderer = std::make_unique<OpenGLRenderer>();
    m_pRenderer->Init();

    ShadowPass* shadowPass = new ShadowPass();
    DefaltForwardPass* pass1 = new DefaltForwardPass();
    DeferredPass* deferredPass = new DeferredPass();
    LightingPass* lightingPass = new LightingPass();
    PostProcessPass* postProcessPass = new PostProcessPass();

    // 注释掉前向渲染Pass，使用延迟渲染管道
    // m_vecPasses.push_back(pass1);
    m_vecPasses.push_back(shadowPass);
    m_vecPasses.push_back(deferredPass);
    m_vecPasses.push_back(lightingPass);
    m_vecPasses.push_back(postProcessPass);

    for (auto pass : m_vecPasses) {
        pass->Init();
    }
    for (auto pass : m_vecPasses) {
        pass->OnResize(m_nWidth, m_nHeight);
    }
    // G-Buffer和阴影贴图现在在每帧通过RenderContext更新
}

/**
 * @brief 反初始化渲染系统
 */
void RenderSystem::OnUninit()
{
    for (auto pass : m_vecPasses) {
        pass->Release();
    }
    m_vecPasses.clear();
    if (m_pRenderer) {
        m_pRenderer->Shutdown();
    }
}

/**
 * @brief 更新渲染系统
 */
void RenderSystem::OnUpdate()
{
    for (auto pass : m_vecPasses) {
        pass->Update();
    }
}

/**
 * @brief 渲染
 */
void RenderSystem::OnRender()
{
    _UpdateRenderContext();
    
    // 绑定imgui窗口的FBO进行渲染
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, m_nWidth, m_nHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    for (auto pass : m_vecPasses) {
        pass->Render();
    }
    
    // 恢复到主窗口FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief 处理窗口调整大小
 * @param renderWidth 渲染宽度
 * @param renderHeight 渲染高度
 * @param windowWidth 窗口宽度
 * @param windowHeight 窗口高度
 */
void RenderSystem::OnResize(uint32_t renderWidth, uint32_t renderHeight, uint32_t windowWidth, uint32_t windowHeight)
{
    if (renderWidth == -1 || renderHeight == -1) {
        return;
    }
    _ResizeColorAttachment(renderWidth, renderHeight);
    for (auto pass : m_vecPasses) {
        pass->OnResize(renderWidth, renderHeight);
    }
}

/**
 * @brief 更新渲染上下文
 */
void RenderSystem::_UpdateRenderContext()
{
    m_sRenderContext.scene = SceneManager::GetActiveScene();
    if (m_sRenderContext.scene) {
        m_sRenderContext.camera = m_sRenderContext.scene->getCamera();
        m_sRenderContext.lights = m_sRenderContext.scene->getLights();
        m_sRenderContext.renderWidth = m_nWidth;
        m_sRenderContext.renderHeight = m_nHeight;
    }
}

/**
 * @brief 获取渲染上下文
 * @return 渲染上下文引用
 */
RenderContext& RenderSystem::GetRenderContext()
{
    return Engine::GetEngine()->GetSystem<RenderSystem>()->m_sRenderContext;
}

/**
 * @brief 设置渲染目标
 * @param texture 纹理
 * @param width 宽度
 * @param height 高度
 */
void RenderSystem::SetRenderTarget(std::shared_ptr<Texture> texture, int width, int height)
{
    m_pTexture = texture;
    m_nWidth = width;
    m_nHeight = height;

    // 1. 创建FBO对象
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // 2. 创建颜色缓冲并附加到FBO
    glGenRenderbuffers(1, &colorBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);

    // 3. 创建深度缓冲并附加到FBO
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetHandle(), 0);

    // 检查FBO完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // 检查失败
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief 调整颜色附件大小
 * @param width 宽度
 * @param height 高度
 */
void RenderSystem::_ResizeColorAttachment(uint32_t width, uint32_t height)
{
    if (fbo != -1 && (width != m_nWidth || height != m_nHeight)) {
        m_nWidth = width;
        m_nHeight = height;
        glBindTexture(GL_TEXTURE_2D, m_pTexture->GetHandle());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 2. 创建颜色缓冲并附加到FBO
        glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);

        // 3. 创建深度缓冲并附加到FBO
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetHandle(), 0);

		// 检查FBO完整性
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		{
			JNLOGERROR("Framebuffer is not complete!\n{}\n");
			return;
		}

		glViewport(0, 0, width, height);

		Camera* pMainCamera = m_sRenderContext.scene->getCamera();
		pMainCamera->setAspectRatio((float)width / (float)height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}

