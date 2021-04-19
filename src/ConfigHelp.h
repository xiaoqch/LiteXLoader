#pragma once
#include <windows.h>
#include <string>

#define LXL_CONFIG_PATH "./plugins/LiteXLoader/config.ini"
#define LXL_CONFIG_MAX_LEN 1024

namespace conf
{
	std::string getStr(const std::string &sec, const std::string &key,
		const std::string &default = "")
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
}