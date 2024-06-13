#pragma once
#include "core.h"
#include <memory>
class Window;
class JNAPI Engine
{
public:
	Engine();
	~Engine();
	void start(); 
	void render();
private:
	std::unique_ptr<Window> window;
};
