#pragma once
#include "pch.h"

#define LXL_VERSION "0.1.0"

#ifdef SCRIPTX_LANG_JAVASCRIPT
	#define LXL_SCRIPT_LANG_TYPE "Js"
    #define LXL_PLUGINS_SUFFIX ".js"
#elif defined(SCRIPTX_LANG_LUA)
	#define LXL_SCRIPT_LANG_TYPE "Lua"
    #define LXL_PLUGINS_SUFFIX ".lua"
#endif

#define LXL_DEF_LOAD_PATH "./plugins"
#define LXL_CONFIG_PATH "./plugins/LiteXLoader/config.ini"
#define LXL_CONFIG_MAX_LEN 1024


namespace conf
{
	std::string getString(const std::string &sec, const std::string &key,
		const std::string &default = "")
	{
		char *buf = new char[LXL_CONFIG_MAX_LEN];
		memset(buf,0,LXL_CONFIG_MAX_LEN);
		bool isOk = GetPrivateProfileString(sec.c_str(),key.c_str(),default.c_str(),buf,LXL_CONFIG_MAX_LEN,LXL_CONFIG_PATH) > 0;
		delete buf;
		return isOk ? std::string(buf) : default;
	}

	int getInt(const std::string &sec, const std::string &key, int default = 0)
	{
		return int(GetPrivateProfileInt(sec.c_str(),key.c_str(),default,LXL_CONFIG_PATH));
	}

	bool getBool(const std::string &sec, const std::string &key, bool default = false)
	{
		std::string_view res{getString(sec,key,"")};
		return res == "1" || res == "true" || res == "True" || res == "TRUE";
	}
}