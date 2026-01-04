#include "StringUtil.h"
#include <algorithm>

namespace StringUtil
{
	std::string GetFileExt(const std::string& strFile, int isLower)
	{
		if (isLower == 1)
		{
			std::string strTemp = strFile;
			std::transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::tolower);
			std::string::size_type pos = strTemp.rfind('.');
			std::string strExt = strTemp.substr(pos == std::string::npos ? strTemp.length() : pos + 1);
			return strExt;
		}
		else
		{
			std::string::size_type pos = strFile.rfind('.');
			std::string strExt = strFile.substr(pos == std::string::npos ? strFile.length() : pos + 1);
			return strExt;
		}
	}
	std::string CombineFilePath(const std::string& rootPath, const std::string& relativePath)
	{
        if (rootPath.empty()) {
            return relativePath;
        }
        if (relativePath.empty()) {
            return rootPath;
        }

        std::string finalPath = rootPath;
        char lastChar = finalPath.back(); // 获取根路径最后一个字符

        // 判断根路径末尾是否已有路径分隔符（Windows:\  Linux:/）
        bool hasSeparator = (lastChar == '\\' || lastChar == '/');

        // 若没有分隔符，自动补充对应系统的路径分隔符
        if (!hasSeparator) {
#ifdef _WIN32 // Windows系统
            finalPath += '\\';
#else // Linux/Mac系统
            finalPath += '/';
#endif
        }

        finalPath += relativePath;
        return finalPath;
	}
}