#include "engine.h"
#include "window.h"
#include "Model.h"
#include "SoftRenderer.h"
#include "Scene.h"

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

	renderer = std::make_shared<SoftRenderer>();
	scene = std::make_shared <Scene>();
	renderer->init(scene, 640, 640);
	scene->init();
	scene->Add(&bunny);

	while (!window->ShouldClose())
	{
		
		render();
		renderer->update();
		renderer->render();
		renderer->present();
		window->update();
	
	}
	window->release();

}


void Engine::render()
{
};