#pragma once
#include "Base/core.h"
#include "IPass.h"
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include <gl/GL.h>
#include <memory>
class Triangle;
class Scene;
class Camera;
class ShaderModule;
class Texture;
class RenderSystem;
class JNAPI DeferredPass : public IPass
{
private:
	std::shared_ptr<ShaderModule> m_pShaderModule = nullptr;
	float frame = 0;
	std::shared_ptr<Texture> texture1 = nullptr;
	std::shared_ptr<Texture> m_gBuffer1 = nullptr; //position
	std::shared_ptr<Texture> m_gBuffer2 = nullptr; //normal
	std::shared_ptr<Texture> m_gBuffer3 = nullptr; //albedo
	std::shared_ptr<Texture> m_gBuffer4 = nullptr; //metallic and roughness

	RenderSystem* m_pRenderSystem = nullptr;
public:
	DeferredPass();
	~DeferredPass();

	void Init() override;

	// 通过 IPass 继承
	void Update() override;
	void Render() override;
	void Release() override;
	void OnResize(uint32_t renderWidth, uint32_t renderHeight) override;

	std::shared_ptr<Texture> GetGBuffer1() const { return m_gBuffer1; }
	std::shared_ptr<Texture> GetGBuffer2() const { return m_gBuffer2; }
	std::shared_ptr<Texture> GetGBuffer3() const { return m_gBuffer3; }
	std::shared_ptr<Texture> GetGBuffer4() const { return m_gBuffer4; }
private:
	unsigned int m_uGbufferFBO = -1;
	unsigned int m_depthRBO = -1;
private:
	void _RenderScene(std::shared_ptr<Scene> scene);
	auto _RecreateRenderResource() -> bool;

};