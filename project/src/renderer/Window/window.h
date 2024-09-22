#pragma once
#include "Base/core.h"
#include "Base/ISystem.h"
class GLFWwindow;
class JNAPI Window :public ISystem
{
private:
	GLFWwindow* m_pWindow = nullptr;
public:
	Window();
	~Window();
public:

	virtual void OnInit();
	virtual void OnUninit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnGui();
	virtual void BeforePresent();
	void Present();

	GLFWwindow* GetGLFWWindow() { return m_pWindow;};
public:
	bool ShouldClose();
private:
};