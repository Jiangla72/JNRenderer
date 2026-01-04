#pragma once
#include "Base/core.h"
#include "IPass.h"
#include <memory>
#include <glm/glm.hpp>

class ShaderModule;
class Texture;

class JNAPI ShadowPass : public IPass
{
private:
    std::shared_ptr<ShaderModule> m_pShadowShader = nullptr;
    unsigned int m_uShadowFBO = 0;
    unsigned int m_uShadowMap = 0;
    int m_nShadowMapSize = 1024;

public:
    ShadowPass();
    ~ShadowPass();

    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnResize(uint32_t renderWidth, uint32_t renderHeight) override;

    unsigned int GetShadowMap() const { return m_uShadowMap; }
    glm::mat4 GetLightSpaceMatrix() const;
};