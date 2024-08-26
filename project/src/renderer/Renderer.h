#pragma once
#include "core.h"
#include <glm/glm.hpp>
#include<vector>
#include<glad/glad.h>
#include<gl/GL.h>
#include <memory>
class Triangle;
class Scene;
class Camera;
class Shader;
class Texture;
class JNAPI Renderer
{
private:
	unsigned int VBO, VAO;
	Shader* m_pShader = nullptr;
	float frame = 0;
	Texture* texture1 = nullptr;
	//struct Light
	//{
	//	glm::vec3 position;
	//	float padding1 = 0;
	//	glm::vec3 intensity;
	//	float padding2 = 0;
	//} light;
public:
	Renderer();
	~Renderer();

	void init();
	void render(std::shared_ptr<Scene> scene);
};