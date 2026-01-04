#pragma once
#include "Base/core.h"
#include <string>
namespace StringUtil
{
	std::string GetFileExt(const std::string& strFile, int isLower = 0);
	std::string CombineFilePath(const std::string& rootPath, const std::string& relativePath);

};