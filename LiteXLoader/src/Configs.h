#pragma once
#include <ScriptX/ScriptX.h>

////////////// Configs //////////////

// 版本记录
#define LXL_VERSION_MAJOR 0
#define LXL_VERSION_MINOR 0
#define LXL_VERSION_BUILD 7
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
#define LXL_DEF_LOAD_DIR "./plugins"
#define LXL_LANGPACK_DIR "./plugins/LiteXLoader/LangPack/"
#define LXL_CONFIG_PATH "./plugins/LiteXLoader/config.ini"

// 基础库 & 依赖库
#define LXL_SCRIPT_DEPENDS_DIR "./plugins/lib"
#define LXL_SCRIPT_BASE_LIB_PATH "./plugins/LiteXLoader/BaseLib" ## LXL_PLUGINS_SUFFIX

// 全局调试
#if defined(SCRIPTX_LANG_JAVASCRIPT)
	#define LXL_DEBUG_CMD "jsdebug"
#elif defined(SCRIPTX_LANG_LUA)
	#define LXL_DEBUG_CMD "luadebug"
#endif

// 全局通信
#define LXL_GLOBAL_DATA_NAME L"LITEXLOADER_GLOBAL_DATA_SECTION"
#define LXL_REMOTE_CALL_EVENT_NAME L"LITEXLOADER_REMOTE_CALL_EVENT"
#define LXL_REMOTE_CALL_MAX_WAIT 120