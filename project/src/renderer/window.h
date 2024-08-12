#pragma once
#include "core.h"

class GLFWwindow;
class JNAPI Window
{
private:
	GLFWwindow* m_pWindow = nullptr;
public:
	Window();
	~Window();
public:
	void init();
	void update();
	void release();
	void present();
public:
	bool ShouldClose();
private:
	void _ProcessInput(GLFWwindow* window);
};