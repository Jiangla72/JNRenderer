#pragma once
#include "Base/core.h"
#include "Base/ISystem.h"
#include <vector>
#include <memory>


class IPass;
class Scene;
class Texture;
struct RenderContext
{
	std::shared_ptr<Scene> scene = nullptr;
};

class JNAPI RenderSystem :public ISystem
{
private:
	unsigned int fbo = 0; // FBO对象的句柄
	unsigned int colorBuffer = 0; // 颜色缓冲区对象的句柄
	unsigned int depthBuffer = 0; // 深度缓冲区对象的句柄
	int m_nWidth = 1000; // FBO的宽度
	int m_nHeight = 600; // FBO的高度
	std::vector<IPass*> m_vecPasses;
	RenderContext m_sRenderContext;
	std::shared_ptr<Texture> m_pTexture = nullptr;
public:
	RenderSystem();
	~RenderSystem();
	static RenderContext& GetRenderContext();

	void SetRenderTarget(std::shared_ptr<Texture> texture,int width, int height);
	void SetRenderContext(RenderContext& renderContext) { m_sRenderContext = renderContext; };


	static unsigned int GetMainFBO();
public:
	virtual void OnInit();
	virtual void OnUninit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnGui() {}
	virtual void OnResize(uint32_t renderWidth, uint32_t renderHeight, uint32_t windowWidth, uint32_t windowHeight);
	virtual void BeforePresent() {}

private:
	void _UpdateRenderContext();
	void _ResizeColorAttachment(uint32_t width, uint32_t height);

};