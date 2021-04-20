#include "ConfigHelp.h"
#include <windows.h>
#include <string>

namespace conf
{
	std::string getString(const std::string &sec, const std::string &key,
		const std::string &default)
	{
		char *buf = new char[LXL_CONFIG_MAX_LEN];
		memset(buf,0,LXL_CONFIG_MAX_LEN);
		bool isOk = GetPrivateProfileString(sec.c_str(),key.c_str(),default.c_str(),buf,LXL_CONFIG_MAX_LEN,LXL_CONFIG_PATH) > 0;
		delete buf;
		return isOk ? std::string(buf) : default;
	}

	int getInt(const std::string &sec, const std::string &key, int default)
	{
		return int(GetPrivateProfileInt(sec.c_str(),key.c_str(),default,LXL_CONFIG_PATH));
	}

	bool getBool(const std::string &sec, const std::string &key, bool default)
	{
		std::string_view res{getString(sec,key,"")};
		return res == "1" || res == "true" || res == "True" || res == "TRUE";
	}
}