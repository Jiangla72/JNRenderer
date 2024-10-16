#pragma once
#include "Base/core.h"
#include "Base/LogSystem.h"
#include "Base/ISystem.h"
#include <memory>
#include <vector>
#include <unordered_map>
int main(int argc, char** argv);

class Window;
class SoftRenderer;
class DefaltForwardPass;
class Scene;
class JNAPI Engine
{
public:
	Engine();
	virtual ~Engine();
	
public:

	static std::shared_ptr <Engine> GetEngine();

	template <typename T>
	bool RegisterSystem()
	{
		static_assert(std::is_base_of<ISystem, T>::value, "class T should extend from IResource");
		size_t hashcode = typeid(T).hash_code();
		if (m_mapSystems.find(hashcode) != m_mapSystems.end())
		{
			JNLOGERROR("ERROR::重复注册System:{}", typeid(T).name());
			return false;
		}

		std::shared_ptr<T> t = std::make_shared<T>();
		m_vecSystems.push_back(t);
		m_mapSystems.emplace(hashcode, t);
		return true;
	}
	template <typename T>
	std::shared_ptr<T> GetSystem()
	{
		size_t hashcode = typeid(T).hash_code();
		if (m_mapSystems.find(hashcode) == m_mapSystems.end())
		{
			JNLOGERROR("ERROR::未注册的System:{}", typeid(T).name());
			return nullptr;
		}

		return std::dynamic_pointer_cast<T>(m_mapSystems[hashcode]);
	}

	void Resize(uint32_t renderWidth, uint32_t renderHeight, uint32_t windowWidth, uint32_t windowHeight);
private:
	void start();

protected:
	virtual void _init();
	virtual void _uninit();
	virtual void _render();
	/// <summary>
	/// 逻辑帧更新，每帧渲染之前调用
	/// </summary>
	virtual void _update();

	virtual void _onGui();

	virtual void _present();
private:
	std::vector<std::shared_ptr<ISystem>> m_vecSystems;
	std::unordered_map<size_t, std::shared_ptr<ISystem>> m_mapSystems;
	//std::shared_ptr<SoftRenderer> renderer = nullptr;
	std::shared_ptr<DefaltForwardPass> renderer = nullptr;
	std::shared_ptr<Scene> scene = nullptr;
	bool m_Running = true;

	friend int ::main(int argc, char** argv);

protected:
	static std::shared_ptr <Engine> Instance;

};

void CreateEngine();

