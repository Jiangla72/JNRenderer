#include "engine.h"
#include "Base/LogSystem.h"

#include "Window/window.h"
#include "Render/Model.h"
#include "Render/SoftRenderer.h"
#include "Render/Renderer.h"
#include "Render/Light.h"
#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
std::shared_ptr <Engine> Engine::Instance = nullptr;
Engine::Engine()
{
}

Engine::~Engine()
{
}

std::shared_ptr <Engine> Engine::GetEngine()
{
	return Instance;
}

void Engine::RegisterSystem()
{
}

Window& Engine::GetWindow()
{
	return *m_Window;
}

void Engine::start()
{
	LogSystem::init();
	JNLOGINFO("LogSystem Inited.");
	ResourceManager::Init();
	JNLOGINFO("ResManager Inited.");

	m_Window = std::make_unique<Window>();
	m_Window->init();


	std::string pathModel = "D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_triangulated_good.obj";
	std::shared_ptr<Model> bunny = ResourceManager::GetResource<Model>(pathModel);
	JNLOGINFO("Create Model at path : {} , with mesh count : {}", pathModel,bunny->m_vecMesh.size());

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
	while (!m_Window->ShouldClose())
	{
		_update();



		
		_render();



		
	
		_present();
	}
	m_Window->release();

}

void Engine::_render(){
	renderer->render(scene);
};

void Engine::_update()
{
	m_Window->update();
	scene->update();
}

void Engine::_present()
{
	m_Window->present();
}
