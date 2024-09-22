#pragma once
#include "Base/core.h"
#include "Base/ISystem.h"
#include <unordered_map>
#include <vector>
#include <memory>
class Scene;
class JNAPI SceneManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<Scene>> m_umapScenes;
	std::vector<std::shared_ptr<Scene>> m_vecScenes;
	std::shared_ptr<Scene> m_pActiveScene = nullptr;

	static std::shared_ptr<SceneManager> Instance;
public:
	SceneManager();
	~SceneManager();

public:
	static void Init();
	static void AddScene(const std::string& name, std::shared_ptr<Scene> scene);
	static void SetActiveScene(const std::string& name);
	static std::shared_ptr<Scene> GetActiveScene();
private:
	void _AddScene(const std::string& name, std::shared_ptr<Scene> scene);
	void _SetActiveScene(const std::string& name);
	std::shared_ptr<Scene> _GetActiveScene();
};