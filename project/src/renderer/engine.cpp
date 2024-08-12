#include "engine.h"
#include "window.h"
#include "Model.h"
#include "SoftRenderer.h"
#include "Scene.h"
#include"Renderer.h"
Engine* Engine::engine = nullptr;
Engine::Engine()
{
}

Engine::~Engine()
{
}

Engine* Engine::getEngine()
{
	if (engine==nullptr)
	{
		engine = new Engine();
	}
	return engine;
}

void Engine::start()
{

	window = std::make_unique<Window>();
	window->init();

	Model bunny("D:\\Workspace\\JNRenderer\\JNRenderer\\models\\bunny\\bunny.obj");

	//renderer = std::make_shared<SoftRenderer>();
	renderer = std::make_shared<Renderer>();
	scene = std::make_shared <Scene>();
	renderer->init();
	scene->init();
	scene->Add(&bunny);

	while (!window->ShouldClose())
	{
		_update();





		_render();



		
	
		_present();
	}
	window->release();

}


void Engine::_render()
{
	renderer->render(scene);
};

void Engine::_update()
{
	window->update();
	scene->update();
}

void Engine::_present()
{
	window->present();
}
