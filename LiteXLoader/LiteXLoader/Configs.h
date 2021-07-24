#pragma once
#include <ScriptX/ScriptX.h>

////////////// Configs //////////////

// 版本记录
#define LXL_VERSION_MAJOR 0
#define LXL_VERSION_MINOR 2
#define LXL_VERSION_BUILD 1
#define LXL_VERSION_IS_BETA true

// 后端宏
#define LXL_LANG_JS "Js"
#define LXL_LANG_LUA "Lua"

#if defined(SCRIPTX_LANG_JAVASCRIPT)
	#define LXL_SCRIPT_LANG_TYPE LXL_LANG_JS
	#define LXL_PLUGINS_SUFFIX ".js"
#elif defined(SCRIPTX_LANG_LUA)
	#define LXL_SCRIPT_LANG_TYPE LXL_LANG_LUA
	#define LXL_PLUGINS_SUFFIX ".lua"
#endif

// 配置文件
#define LXL_PLUGINS_LOAD_DIR "./plugins"
#define LXL_LANGPACK_DIR "./plugins/LiteXLoader/LangPack/"
#define LXL_CONFIG_PATH "./plugins/LiteXLoader/config.ini"

// 基础库 & 依赖库
#define LXL_DEPENDS_DIR "./plugins/lib"

// 全局调试
#if defined(SCRIPTX_LANG_JAVASCRIPT)
	#define LXL_DEBUG_CMD "jsdebug"
#elif defined(SCRIPTX_LANG_LUA)
	#define LXL_DEBUG_CMD "luadebug"
#endif
#define LXL_DEBUG_ENGINE_NAME "__LXL_DEBUG_ENGINE__"

// 热管理命令
#define LXL_HOT_MANAGE_PREFIX "lxl"
#define LXL_HOT_LIST "lxl list"
#define LXL_HOT_LOAD "lxl load"
#define LXL_HOT_UNLOAD "lxl unload"
#define LXL_HOT_RELOAD "lxl reload"

// 全局通信
#define LXL_GLOBAL_DATA_NAME L"LITEXLOADER_GLOBAL_DATA_SECTION"
#define LXL_REMOTE_CALL_EVENT_NAME L"LITEXLOADER_REMOTE_CALL_EVENT"
#define LXL_REMOTE_CALL_MAX_WAIT 120