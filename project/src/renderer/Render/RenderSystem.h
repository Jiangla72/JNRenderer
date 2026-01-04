#pragma once
#include "Base/core.h"
#include "Base/ISystem.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class IPass;
class Scene;
class Texture;
class Camera;
struct Light;
class IRenderer;

struct RenderContext
{
	std::shared_ptr<Scene> scene = nullptr;
	Camera* camera = nullptr;
	std::vector<Light*> lights;
	int renderWidth = 0;
	int renderHeight = 0;
	std::shared_ptr<Texture> m_gBuffer1 = nullptr; //position
	std::shared_ptr<Texture> m_gBuffer2 = nullptr; //normal
	std::shared_ptr<Texture> m_gBuffer3 = nullptr; //albedo
	std::shared_ptr<Texture> m_gBuffer4 = nullptr; //metallic and roughness
	unsigned int m_uShadowMap = 0; //shadow map
	glm::mat4 m_lightSpaceMatrix = glm::mat4(1.0f); //light space matrix for shadow mapping
};

class JNAPI RenderSystem :public ISystem
{
private:
	std::unique_ptr<IRenderer> m_pRenderer; // 渲染器抽象层
	unsigned int fbo = 0;                  // 主渲染 FBO 句柄
	unsigned int colorBuffer = 0;          // 主渲染 FBO 的颜色缓冲
	unsigned int depthBuffer = 0;          // 主渲染 FBO 的深度缓冲
	int m_nWidth = 1000;                   // 主渲染区域宽度
	int m_nHeight = 600;                   // 主渲染区域高度
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
