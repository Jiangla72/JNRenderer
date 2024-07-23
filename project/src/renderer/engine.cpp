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

	SoftRenderer* renderer =new SoftRenderer();
	renderer->init(640,640);
	Scene* scene=new Scene();
	scene->init();
	scene->Add(&bunny);

	while (!window->ShouldClose())
	{
		
		render();
		renderer->render(scene);
		renderer->present();
		window->update();
	}
	window->release();

	delete renderer;
	delete scene;
}


void Engine::render()
{
};