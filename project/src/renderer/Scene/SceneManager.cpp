#include "SceneManager.h"
#include "Base/LogSystem.h"
#include "Scene/Scene.h"
#include "engine.h"

std::shared_ptr<SceneManager> SceneManager::Instance = nullptr;
SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	if (Instance == nullptr)
	{
		Instance = std::make_shared<SceneManager>();
	}
}

void SceneManager::AddScene(const std::string& name, std::shared_ptr<Scene> scene)
{
	SceneManager::Instance->_AddScene(name, scene);
}

void SceneManager::SetActiveScene(const std::string& name)
{
	SceneManager::Instance->_SetActiveScene(name);
}

std::shared_ptr<Scene> SceneManager::GetActiveScene()
{
	return SceneManager::Instance->_GetActiveScene();
}

void SceneManager::_AddScene(const std::string& name, std::shared_ptr<Scene> scene)
{
	if (m_umapScenes.find(name) != m_umapScenes.end())
	{
		JNLOGERROR("重复添加Scene : {}", name);
		return;
	}

	m_umapScenes.emplace(name, scene);
}

void SceneManager::_SetActiveScene(const std::string& name)
{
	if (m_umapScenes.find(name) == m_umapScenes.end())
	{
		JNLOGERROR("Scene不存在 : {}", name);
		return;
	}
	if (m_pActiveScene)
	{
		m_pActiveScene->OnDeactive();
	}
	m_pActiveScene = m_umapScenes[name];
	if (!m_pActiveScene)
	{
		JNLOGERROR("Scene为空 : {}", name);
		return;
	}
	m_pActiveScene->OnActive();
}

std::shared_ptr<Scene> SceneManager::_GetActiveScene()
{
	return m_pActiveScene;
}
