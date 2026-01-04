#include "ShadowPass.h"
#include "Base/Config.h"
#include "Render/ShaderModule.h"
#include "Render/Camera.h"

#include "Render/Texture.h"
#include "Render/RenderSystem.h"
#include "Render/Light.h"
#include "Render/Model.h"
#include "Render/Mesh.h"
#include "Scene/Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
ShadowPass::ShadowPass() {}

ShadowPass::~ShadowPass() {}

void ShadowPass::Init()
{
    // 创建阴影着色器（简单深度pass）
    m_pShadowShader = ShaderModule::GetShaderModule(
        Config::GetShaderPath("shadow", "vertex").c_str(),
        Config::GetShaderPath("shadow", "fragment").c_str()
    );

    // 创建阴影贴图
    glGenFramebuffers(1, &m_uShadowFBO);
    glGenTextures(1, &m_uShadowMap);
    glBindTexture(GL_TEXTURE_2D, m_uShadowMap);
    m_nShadowMapSize = Config::GetShadowMapSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Config::GetShadowMapSize(), Config::GetShadowMapSize(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // 启用硬件比较（PCF）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glBindFramebuffer(GL_FRAMEBUFFER, m_uShadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uShadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowPass::Update() {}

void ShadowPass::Render()
{
    glViewport(0, 0, m_nShadowMapSize, m_nShadowMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uShadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    //glDisable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    m_pShadowShader->use();

    // 设置光空间矩阵
 
    const glm::mat4& lightSpaceMatrix = GetLightSpaceMatrix();
    m_pShadowShader->setUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);

    // 渲染场景到阴影贴图
    RenderContext& renderContext = RenderSystem::GetRenderContext();
    
    // 更新RenderContext中的阴影数据
    renderContext.m_uShadowMap = m_uShadowMap;
    renderContext.m_lightSpaceMatrix = GetLightSpaceMatrix();
    
    if (renderContext.scene) {
        for (auto& model : renderContext.scene->getModels()) {
            // 为每个模型设置其 model 矩阵
            m_pShadowShader->setUniformMatrix4fv("model", model->GetModelMatrix());
            model->render();
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE);
}

void ShadowPass::Release()
{
    if (m_uShadowFBO) glDeleteFramebuffers(1, &m_uShadowFBO);
    if (m_uShadowMap) glDeleteTextures(1, &m_uShadowMap);
}

void ShadowPass::OnResize(uint32_t renderWidth, uint32_t renderHeight)
{
    // 阴影pass不依赖窗口大小
}

glm::mat4 ShadowPass::GetLightSpaceMatrix() const
{
    auto& rc = RenderSystem::GetRenderContext();
    if (rc.lights.empty() || !rc.camera) return glm::mat4(1.0f);

    Light* light = rc.lights[0];
    glm::vec3 camPos = glm::vec3(rc.camera->getCameraData().pos);
    glm::vec3 camFwd = glm::normalize(glm::vec3(rc.camera->getCameraData().forward));

    // 方向光处理：position 作为方向向量（w==0）
    glm::vec3 lightDir = glm::normalize(glm::vec3(light->position));
    if (light->position.w != 0.0f) {
        // 点光近似方向
        lightDir = glm::normalize(camPos - glm::vec3(light->position));
    }

    // 将光源观察中心放在相机前方一定距离，提升可见区域的覆盖率
    glm::vec3 center = camPos + camFwd * 5.0f;
    glm::vec3 lightPos = center - lightDir * 20.0f;

    // 简单正交包围盒
    float extent = 20.0f;
    glm::mat4 lightProjection = glm::ortho(-extent, extent, -extent, extent, 1.0f, 80.0f);
    glm::mat4 lightView = glm::lookAt(lightPos, center, glm::vec3(0.0f, 1.0f, 0.0f));

    // Texel snapping：将光空间中心对齐到阴影贴图的 texel 网格，减少相机移动带来的抖动
    float worldUnitsPerTexel = (extent * 2.0f) / float(m_nShadowMapSize);
    glm::vec4 centerLS = lightView * glm::vec4(center, 1.0f);
    glm::vec2 snapped = glm::vec2(
        std::floor(centerLS.x / worldUnitsPerTexel) * worldUnitsPerTexel,
        std::floor(centerLS.y / worldUnitsPerTexel) * worldUnitsPerTexel
    );
    glm::vec2 deltaLS = glm::vec2(snapped.x - centerLS.x, snapped.y - centerLS.y);
    glm::vec3 deltaWS = glm::vec3(glm::inverse(lightView) * glm::vec4(deltaLS.x, deltaLS.y, 0.0f, 0.0f));
    center += deltaWS;
    lightPos += deltaWS;
    lightView = glm::lookAt(lightPos, center, glm::vec3(0.0f, 1.0f, 0.0f));
    return lightProjection * lightView;
}
