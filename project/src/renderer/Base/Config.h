#pragma once
#include "Base/core.h"
#include <string>
#include "json.hpp"

class JNAPI Config
{
private:
    static nlohmann::json m_config;
    static bool m_loaded;

public:
    static bool Load(const std::string& filePath);
    static std::string GetRootPath();
    static std::string GetShaderPath(const std::string& shaderName, const std::string& type);
    static std::string GetModelTexturePath(const std::string& modelName);
    static int GetWindowWidth();
    static int GetWindowHeight();
    static int GetShadowMapSize();
};