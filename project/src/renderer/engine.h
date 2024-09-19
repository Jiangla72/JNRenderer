#pragma once
#include "Base/core.h"
#include <memory>
#include <vector>
int main(int argc, char** argv);

class Window;
class SoftRenderer;
class Renderer;
class Scene;
class ISystem;
class JNAPI Engine
{
public:
	Engine();
	virtual ~Engine();
	

public:
	static std::shared_ptr <Engine> GetEngine();

	void RegisterSystem();
	Window& GetWindow();
private:
	void start();

private:
	void _render();
	/// <summary>
	/// 逻辑帧更新，每帧渲染之前调用
	/// </summary>
	void _update();

	void _present();
private:
	std::vector<ISystem*> m_Systems;
	std::unique_ptr<Window> m_Window;
	//std::shared_ptr<SoftRenderer> renderer = nullptr;
	std::shared_ptr<Renderer> renderer = nullptr;
	std::shared_ptr<Scene> scene = nullptr;
	bool m_Running = true;

	static std::shared_ptr <Engine> Instance;
	friend int ::main(int argc, char** argv);

};

std::shared_ptr<Engine> CreateEngine();