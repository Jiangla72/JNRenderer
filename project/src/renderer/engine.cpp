#include "engine.h"
#include "window.h"
Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::start()
{

	window = std::make_unique<Window>();
	window->init();
	
	while (!window->ShouldClose())
	{
		window->update();
		render();
	}
	window->release();
}


void Engine::render()
{
};