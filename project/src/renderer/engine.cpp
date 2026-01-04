#include "engine.h"
#include "Base/Timer.h"
#include "Base/Config.h"
#include "Window/window.h"
#include "Render/Model.h"
#include "Render/SoftRenderer.h"
#include "Render/RenderSystem.h"
#include "Render/Light.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
std::shared_ptr <Engine> Engine::Instance = nullptr;
Engine::Engine()
{
	Instance = std::shared_ptr <Engine>(this);
}

Engine::~Engine()
{
	
}

std::shared_ptr <Engine> Engine::GetEngine()
{
	return Instance;
}

void Engine::Resize(uint32_t renderWidth, uint32_t renderHeight, uint32_t windowWidth, uint32_t windowHeight)
{
	for (auto system : m_vecSystems)
		system->OnResize(renderWidth, renderHeight, windowWidth, windowHeight);
}

void Engine::start()
{
	LogSystem::init();
	JNLOGINFO("LogSystem Inited.");
	if (!Config::Load("G:\\JNRenderer\\JNRenderer\\config.json")) {
		JNLOGERROR("Failed to load config.json");
	}
	JNLOGINFO("Config loaded.");
	ResourceManager::Init();
	JNLOGINFO("ResManager Inited.");
	SceneManager::Init();
	JNLOGINFO("SceneManager Inited.");

	RegisterSystem<Window>();
	RegisterSystem<RenderSystem>();
	_init();

	while (!GetSystem<Window>()->ShouldClose())
	{
		Timer::Tick();
		_update();



		
		_render();

		_onGui();

		
	
		_present();
	}
	_uninit();
}

void Engine::_init()
{
	for (auto system : m_vecSystems)
		system->OnInit();
}

void Engine::_uninit()
{
	for (auto system : m_vecSystems)
		system->OnUninit();

}

void Engine::_render(){
	for (auto system : m_vecSystems)
		system->OnRender();
};

void Engine::_update()
{
	for (auto system : m_vecSystems)
		system->OnUpdate();
}

void Engine::_onGui()
{
	for (auto system : m_vecSystems)
		system->OnGui();
}

void Engine::_present()
{
	for (auto system : m_vecSystems)
		system->BeforePresent();
	GetSystem<Window>()->Present();
}
