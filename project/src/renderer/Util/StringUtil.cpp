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
}