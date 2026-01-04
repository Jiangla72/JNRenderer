#include "Config.h"
#include "Util/StringUtil.h"
#include <fstream>
#include <iostream>

nlohmann::json Config::m_config;
bool Config::m_loaded = false;

bool Config::Load(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filePath << std::endl;
        return false;
    }
    file >> m_config;
    m_loaded = true;
    return true;
}

std::string Config::GetRootPath()
{
    if (!m_loaded) return "";
    return m_config["RootPath"];
}

std::string Config::GetShaderPath(const std::string& shaderName, const std::string& type)
{
    if (!m_loaded) return "";
    const std::string& rootPath = GetRootPath();
    const std::string& shaderRelativePath = m_config["shaders"][shaderName][type];
    return StringUtil::CombineFilePath(rootPath, shaderRelativePath);
}

std::string Config::GetModelTexturePath(const std::string& modelName)
{
    if (!m_loaded) return "";
    const std::string& rootPath = GetRootPath();
    const std::string& texmodelRelativePath = m_config["models"][modelName]["texture"];

    return StringUtil::CombineFilePath(rootPath, texmodelRelativePath);
}

int Config::GetWindowWidth()
{
    if (!m_loaded) return 1000;
    return m_config["window"]["width"];
}

int Config::GetWindowHeight()
{
    if (!m_loaded) return 600;
    return m_config["window"]["height"];
}

int Config::GetShadowMapSize()
{
    if (!m_loaded) return 1024;
    return m_config["shadow"]["mapSize"];
}