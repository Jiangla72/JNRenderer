#include "engine.h"
#include "window.h"
#include "Model.h"
#include "SoftRenderer.h"
#include "Scene.h"
#include "Renderer.h"
#include "Light.h"
#include "LogSystem.h"
std::shared_ptr <Engine> Engine::engine = nullptr;
Engine::Engine()
{
}

Engine::~Engine()
{
}

std::shared_ptr <Engine> Engine::getEngine()
{
	if (engine==nullptr)
	{
		engine = std::make_shared<Engine>();
	}
	return engine;
}

void Engine::start()
{
	LogSystem::init();
	JNLOGINFO("LogSystem Inited.");
	window = std::make_unique<Window>();
	window->init();


	//Model bunny("D:\\Workspace\\JNRenderer\\JNRenderer\\models\\bunny\\bunny.obj");
	std::string pathModel = "D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_triangulated_good.obj";
	Model* bunny = new Model(pathModel);
	JNLOGERROR("Create Model at path : {} , with mesh count : {}", pathModel,bunny->m_vecMesh.size());
	//renderer = std::make_shared<SoftRenderer>();
	renderer = std::make_shared<Renderer>();
	scene = std::make_shared <Scene>();
	renderer->init();
	scene->init();
	scene->Add(bunny);

	Light* light = new Light();
	light->position = { 20, 0, 50 ,0};
	light->intensity = { 500, 500, 500 ,0 };
	light->color = { 0.2, 0, 0.21 ,0 };
	Light* light1 = new Light();
	light1->position = { 0, 0, 50 ,0 };
	light1->intensity = { 500, 500, 500 ,0 };
	light1->color = { 0.53, 0.12, 0.8 ,0 };

	Light* light2 = new Light();
	light2->position = { 20, 0, 50 ,0 };
	light2->intensity = { 500, 500, 500 ,0 };
	light2->color = { 0.6, 1, 0.6 ,0 };

	scene->Add(light);
	scene->Add(light1);
	scene->Add(light2);
	while (!window->ShouldClose())
	{
		_update();





		_render();



		
	
		_present();
	}
	window->release();

}


void Engine::_render(){
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
