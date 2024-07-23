#pragma once
#include "core.h"
#include <memory>
class Window;
class JNAPI Engine
{
private:
	Engine();
	~Engine();

public:
	static Engine* engine;
	static Engine* getEngine();
	void start(); 
	void render();
private:
	std::unique_ptr<Window> window;
};
