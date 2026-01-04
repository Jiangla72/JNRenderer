#pragma once
#include "Base/core.h"
#include "IPass.h"
#include <memory>
#include <glm/glm.hpp>

class ShaderModule;
class Texture;

/// @brief Post-Processing Pass - 处理后处理效果如 Bloom 和 Tone Mapping
class JNAPI PostProcessPass : public IPass
{
private:
    std::shared_ptr<ShaderModule> m_pBloomShader = nullptr;
    std::shared_ptr<ShaderModule> m_pToneMappingShader = nullptr;
    
    // Bloom 纹理
    unsigned int m_uBloomFBO = 0;
    std::shared_ptr<Texture> m_bloomTexture = nullptr;
    
    // Tone Mapping 纹理
    unsigned int m_uToneMappingFBO = 0;
    std::shared_ptr<Texture> m_toneMappedTexture = nullptr;
    
    // 全屏四边形
    unsigned int m_uQuadVAO = 0, m_uQuadVBO = 0;
    
    // HDR 输入纹理
    std::shared_ptr<Texture> m_hdrTexture = nullptr;
    
    // Bloom 强度和阈值
    float m_fBloomStrength = 0.3f;
    float m_fBloomThreshold = 1.0f;

public:
    PostProcessPass();
    ~PostProcessPass();

    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnResize(uint32_t renderWidth, uint32_t renderHeight) override;

    void SetHDRTexture(std::shared_ptr<Texture> hdrTexture);

private:
    void _CreateFramebuffers();
    void _ApplyBloom();
    void _ApplyToneMapping();
};
