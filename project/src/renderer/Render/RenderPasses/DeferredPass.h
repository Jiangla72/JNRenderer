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
	std::shared_ptr<Texture> m_gBuffer3 = nullptr; //color and specular
	//std::shared_ptr<Texture> m_gBuffer4 = nullptr;

	RenderSystem* m_pRenderSystem = nullptr;
public:
	DeferredPass();
	~DeferredPass();

	void Init() override;

	// Í¨¹ý IPass ¼Ì³Ð
	void Update() override;
	void Render() override;
	void Release() override;
	void OnResize(uint32_t renderWidth, uint32_t renderHeight) override;
private:
	unsigned int m_uGbufferFBO;
private:
	void _RenderScene(std::shared_ptr<Scene> scene);
	auto _RecreateRenderResource() -> bool;

};