#pragma once

#include "Base/core.h"
#include "Base/LogSystem.h"
#include "IResource.h"
#include "Render/Model.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
class JNAPI ResourceManager
{
private:
	std::unordered_map<size_t, std::shared_ptr<IResource>> m_mapPathToRes;
	std::unordered_map<size_t, ILoader*> m_mapTypeToMgr;
public:
	static void Init()
	{
		if (resMgr == nullptr)
		{
			resMgr = std::make_unique<ResourceManager>();
		}
		
	};
public:
	ResourceManager();
	~ResourceManager();

	static std::unique_ptr <ResourceManager> resMgr;
	
public:
	template <typename T>
	auto RegisterRersourceType() -> bool
	{
		static_assert(std::is_base_of<IResource, T>::value, "class T should extend from IResource");
 		switch (T::GetType())
		{
			case eModel:
				if (m_mapTypeToMgr.find(typeid(Model).hash_code()) == m_mapTypeToMgr.end())
				{
					ModelLoader* modelMgr = new ModelLoader();
					m_mapTypeToMgr.emplace(typeid(Model).hash_code(), modelMgr);
				}
			break;
			case eTexture2D:
				if (m_mapTypeToMgr.find(typeid(Texture).hash_code()) == m_mapTypeToMgr.end())
				{
					TextureLoader* textureMgr = new TextureLoader();
					m_mapTypeToMgr.emplace(typeid(Texture).hash_code(), textureMgr);
				}
			break;
			case eShader:
				if (m_mapTypeToMgr.find(typeid(Shader).hash_code()) == m_mapTypeToMgr.end())
				{
					ShaderLoader* shaderMgr = new ShaderLoader();
					m_mapTypeToMgr.emplace(typeid(Shader).hash_code(), shaderMgr);
				}
			break;
			default:
				return false;
				break;
			
		}
		return true;
	};

	template <typename T>
	static std::shared_ptr<T> GetResource(const std::string& resPath)
	{
		if (resPath.empty())
		{
			return nullptr;
		}
		std::hash<std::string> stringHash;
		size_t hash = stringHash(resPath);
		if (resMgr->m_mapPathToRes.find(hash) == resMgr->m_mapPathToRes.end())
		{
			static_assert(std::is_base_of<IResource, T>::value, "class T should extend from IResource");
			std::shared_ptr<T> t = std::make_shared<T>();
			auto count = t.use_count();
			resMgr->m_mapPathToRes.insert(std::pair<uint64_t, std::shared_ptr<IResource>>(hash, t));
			count = t.use_count();
			//find spec loader to load
			ILoader* loader = nullptr;
			if (resMgr->m_mapTypeToMgr.find(typeid(T).hash_code()) == resMgr->m_mapTypeToMgr.end())
			{
				if (!resMgr->RegisterRersourceType<T>())
				{
					JNLOGERROR("×¢²áÊ§°Ü, type : {}", typeid(T).name());
				}
			}
			loader = resMgr->m_mapTypeToMgr[typeid(T).hash_code()];

			if (loader)
			{
				 count = t.use_count();
				loader->Load(resPath, t);
				 count = t.use_count();
			}
			else
			{
				JNLOGERROR("try to load with type bound to none loader , type : {}", typeid(T).name());
			}
			
			return t;
		}
		else
		{
			return std::dynamic_pointer_cast<T>(resMgr->m_mapPathToRes[hash]);
		}
		
	};

};
