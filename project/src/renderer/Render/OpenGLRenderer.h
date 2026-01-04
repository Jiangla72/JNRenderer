#pragma once
#include "IRenderer.h"
#include <glad/glad.h>

class JNAPI OpenGLRenderer : public IRenderer
{
public:
    void Init() override;
    void Shutdown() override;

    void ClearColor(float r, float g, float b, float a) override;
    void Clear(unsigned int flags) override;

    void BindTexture(unsigned int texture, int unit) override;
    void BindFramebuffer(unsigned int fbo) override;

    void DrawArrays(unsigned int mode, int first, int count) override;
    void DrawElements(unsigned int mode, int count, unsigned int type, const void* indices) override;
};