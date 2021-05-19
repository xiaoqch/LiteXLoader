#pragma once
#include "ScriptX.h"

////////////// Configs //////////////
#define LXL_VERSION "0.0.6"

// 后端宏
#if defined(SCRIPTX_LANG_JAVASCRIPT)
	#define LXL_SCRIPT_LANG_TYPE "Js"
	#define LXL_PLUGINS_SUFFIX ".js"
#elif defined(SCRIPTX_LANG_LUA)
	#define LXL_SCRIPT_LANG_TYPE "Lua"
	#define LXL_PLUGINS_SUFFIX ".lua"
#endif

// 配置文件
#define LXL_DEF_LOAD_DIR "./plugins"
#define LXL_CONFIG_PATH "./plugins/LiteXLoader/config.ini"
#define LXL_CONFIG_MAX_LEN 1024

// 基础库 & 依赖库
#define LXL_SCRIPT_DEPENDS_DIR "./plugins/lib"
#define LXL_SCRIPT_BASE_LIB_PATH "./plugins/LiteXLoader/BaseLib" ## LXL_PLUGINS_SUFFIX

// 全局调试
#if defined(SCRIPTX_LANG_JAVASCRIPT)
	#define LXL_DEBUG_CMD "jsdebug"
#elif defined(SCRIPTX_LANG_LUA)
	#define LXL_DEBUG_CMD "luadebug"
#endif