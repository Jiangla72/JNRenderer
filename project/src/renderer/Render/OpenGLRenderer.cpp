#include "OpenGLRenderer.h"

void OpenGLRenderer::Init()
{
    // OpenGL 初始化已在GLAD中处理
}

void OpenGLRenderer::Shutdown()
{
    // 清理资源
}

void OpenGLRenderer::ClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void OpenGLRenderer::Clear(unsigned int flags)
{
    glClear(flags);
}

void OpenGLRenderer::BindTexture(unsigned int texture, int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void OpenGLRenderer::BindFramebuffer(unsigned int fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void OpenGLRenderer::DrawArrays(unsigned int mode, int first, int count)
{
    glDrawArrays(mode, first, count);
}

void OpenGLRenderer::DrawElements(unsigned int mode, int count, unsigned int type, const void* indices)
{
    glDrawElements(mode, count, type, indices);
}