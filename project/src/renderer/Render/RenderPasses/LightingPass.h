#pragma once
#include "Base/core.h"
#include "IPass.h"
#include <memory>
#include <glm/glm.hpp>

class ShaderModule;
class Texture;

class JNAPI LightingPass : public IPass
{
private:
    std::shared_ptr<ShaderModule> m_pLightingShader = nullptr;
    unsigned int m_uQuadVAO = 0, m_uQuadVBO = 0;

    std::shared_ptr<Texture> m_gBuffer1 = nullptr;
    std::shared_ptr<Texture> m_gBuffer2 = nullptr;
    std::shared_ptr<Texture> m_gBuffer3 = nullptr;
    std::shared_ptr<Texture> m_gBuffer4 = nullptr;
    unsigned int m_uShadowMap = 0;
    glm::mat4 m_lightSpaceMatrix;

public:
    LightingPass();
    ~LightingPass();

    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnResize(uint32_t renderWidth, uint32_t renderHeight) override;

    void SetGBuffer(std::shared_ptr<Texture> gBuffer1, std::shared_ptr<Texture> gBuffer2, std::shared_ptr<Texture> gBuffer3, std::shared_ptr<Texture> gBuffer4);
    void SetShadowMap(unsigned int shadowMap, const glm::mat4& lightSpaceMatrix);

private:
    void _RenderLighting();
};