#include "PostProcessPass.h"
#include "Base/Config.h"
#include "Render/ShaderModule.h"
#include "Render/Texture.h"
#include "Render/RenderSystem.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

PostProcessPass::PostProcessPass() {}

PostProcessPass::~PostProcessPass() {}

void PostProcessPass::Init()
{
    // 加载着色器
    m_pBloomShader = ShaderModule::GetShaderModule(
        Config::GetShaderPath("postProcess", "vertex").c_str(),
        Config::GetShaderPath("postProcess", "fragment").c_str()
    );
    m_pToneMappingShader = ShaderModule::GetShaderModule(
        Config::GetShaderPath("toneMapping", "vertex").c_str(),
        Config::GetShaderPath("toneMapping", "fragment").c_str()
    );

    // 创建全屏四边形
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &m_uQuadVAO);
    glGenBuffers(1, &m_uQuadVBO);
    glBindVertexArray(m_uQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_uQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    // 创建帧缓冲和纹理
    _CreateFramebuffers();
}

void PostProcessPass::_CreateFramebuffers()
{
    // Bloom FBO
    if (m_uBloomFBO == 0) {
        glGenFramebuffers(1, &m_uBloomFBO);
    }

    if (!m_bloomTexture) {
        TextureDesc desc{};
        desc.width = m_uWidth;
        desc.height = m_uHeight;
        desc.format = JNF_RGBA;
        m_bloomTexture = TextureHelper::CreateTexture(desc);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_uBloomFBO);
    glBindTexture(GL_TEXTURE_2D, m_bloomTexture->GetHandle());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_uWidth, m_uHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // S 轴（X 轴）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // T 轴（Y 轴）
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bloomTexture->GetHandle(), 0);
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        // FBO配置失败，可打印错误信息调试
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // 解绑 FBO，切换到默认帧缓冲（屏幕）
    glBindTexture(GL_TEXTURE_2D, 0);

    // Tone Mapping FBO
    if (m_uToneMappingFBO == 0) {
        glGenFramebuffers(1, &m_uToneMappingFBO);
    }

    if (!m_toneMappedTexture) {
        TextureDesc desc{};
        desc.width = m_uWidth;
        desc.height = m_uHeight;
        desc.format = JNF_RGBA;
        m_toneMappedTexture = TextureHelper::CreateTexture(desc);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_uToneMappingFBO);
    glBindTexture(GL_TEXTURE_2D, m_toneMappedTexture->GetHandle());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_toneMappedTexture->GetHandle(), 0);
    //GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        // FBO配置失败，可打印错误信息调试
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PostProcessPass::_ApplyBloom()
{
    if (!m_hdrTexture || !m_pBloomShader) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_uBloomFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_pBloomShader->use();
    if (m_hdrTexture) m_hdrTexture->BindTexture(0);
    m_pBloomShader->useTexture("hdrTexture", 0);

    glBindVertexArray(m_uQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessPass::_ApplyToneMapping()
{
    if (!m_hdrTexture || !m_pToneMappingShader) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_uToneMappingFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_pToneMappingShader->use();
    if (m_hdrTexture) m_hdrTexture->BindTexture(0);
    m_pToneMappingShader->useTexture("hdrTexture", 0);
    
    if (m_bloomTexture) {
        m_bloomTexture->BindTexture(1);
        m_pToneMappingShader->useTexture("bloomTexture", 1);
    }

    glBindVertexArray(m_uQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessPass::Update() {}

void PostProcessPass::Render()
{
    if (!m_hdrTexture) return;

    _ApplyBloom();
    _ApplyToneMapping();
}

void PostProcessPass::SetHDRTexture(std::shared_ptr<Texture> hdrTexture)
{
    m_hdrTexture = hdrTexture;
}

void PostProcessPass::Release()
{
    if (m_uQuadVAO) glDeleteVertexArrays(1, &m_uQuadVAO);
    if (m_uQuadVBO) glDeleteBuffers(1, &m_uQuadVBO);
    if (m_uBloomFBO) glDeleteFramebuffers(1, &m_uBloomFBO);
    if (m_uToneMappingFBO) glDeleteFramebuffers(1, &m_uToneMappingFBO);
    
    if (m_bloomTexture) {
        m_bloomTexture->Release();
        m_bloomTexture.reset();
    }
    if (m_toneMappedTexture) {
        m_toneMappedTexture->Release();
        m_toneMappedTexture.reset();
    }
}

void PostProcessPass::OnResize(uint32_t renderWidth, uint32_t renderHeight)
{
    if (m_uWidth != renderWidth || m_uHeight != renderHeight) {
        m_uWidth = renderWidth;
        m_uHeight = renderHeight;
        _CreateFramebuffers();
    }
}
