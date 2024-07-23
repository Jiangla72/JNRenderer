#pragma once
#include "core.h"
#include <glm/glm.hpp>
#include<vector>
#include<glad/glad.h>
#include<gl/GL.h>
class Triangle;
class Scene;
class Camera;
class JNAPI SoftRenderer
{
private:
	std::vector<float> depth_buf;
	GLubyte* frame_buf;
	GLuint m_backBuffer;
	int m_nWidth = 0;
	int m_nHeight = 0;

	struct light
	{
		glm::vec3 position;
		glm::vec3 intensity;
	};

	unsigned int shaderProgram;
	unsigned int VBO, VAO, EBO;
public:
	SoftRenderer();
	~SoftRenderer();
public:
	void init(int width,int height);
	void render(Scene* scene);
	void present();
private:
	void clear();
	void draw(Scene* scene);
	void rasterize(const Triangle& t, const std::array<glm::vec3, 3>& world_pos);
	glm::vec3 texture_fragment_shader(glm::vec3 pos, glm::vec3 normal);
};