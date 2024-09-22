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
class JNAPI DefaltForwardPass:public IPass
{
private:
	unsigned int VBO, VAO;
	std::shared_ptr<ShaderModule> m_pShaderModule = nullptr;
	float frame = 0;
	std::shared_ptr<Texture> texture1 = nullptr;
	RenderSystem* m_pRenderSystem = nullptr;
public:
	DefaltForwardPass();
	~DefaltForwardPass();

	void Init() override;

	// Í¨¹ý IPass ¼Ì³Ð
	void Update() override;
	void Render() override;
	void Release() override;

private:
	void _RenderScene(std::shared_ptr<Scene> scene);

};