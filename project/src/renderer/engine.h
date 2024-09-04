#pragma once
#include "Base/core.h"
#include <memory>
class Window;
class SoftRenderer;
class Renderer;
class Scene;
class JNAPI Engine
{
public:
	~Engine();
	Engine();
	

public:
	static std::shared_ptr <Engine> engine;
	static std::shared_ptr <Engine> getEngine();
	void start(); 
private:
	void _render();
	/// <summary>
	/// �߼�֡���£�ÿ֡��Ⱦ֮ǰ����
	/// </summary>
	void _update();

	void _present();
private:
	std::unique_ptr<Window> window;
	//std::shared_ptr<SoftRenderer> renderer = nullptr;
	std::shared_ptr<Renderer> renderer = nullptr;
	std::shared_ptr<Scene> scene = nullptr;
	
};
