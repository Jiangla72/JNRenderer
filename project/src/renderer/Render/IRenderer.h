#pragma once
#include "Base/core.h"
#include <memory>

class Texture;
class ShaderModule;

class JNAPI IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void ClearColor(float r, float g, float b, float a) = 0;
    virtual void Clear(unsigned int flags) = 0;

    virtual void BindTexture(unsigned int texture, int unit) = 0;
    virtual void BindFramebuffer(unsigned int fbo) = 0;

    virtual void DrawArrays(unsigned int mode, int first, int count) = 0;
    virtual void DrawElements(unsigned int mode, int count, unsigned int type, const void* indices) = 0;

    // 其他抽象方法可按需添加
};