#include <ScriptX/ScriptX.h>
#include <vector>
#include <list>
#include <string>
#include <map>
using namespace script;

void InitEngineGlobalData(bool* isFirstInstance);

//主引擎表
extern std::list<std::shared_ptr<ScriptEngine>> lxlModules;

//导出函数表
struct ExportedFuncData
{
	std::string fromEngineType;
	ScriptEngine* engine;
	Global<Function> func;
};

//远程调用信息
struct RemoteEngineData
{
	unsigned threadId;
};

//表单回调信息
struct FormCallbackData
{
	script::ScriptEngine* engine;
	script::Global<script::Function> func;
};

struct GlobalDataType
{
	//所有插件名单
	std::vector<std::string> pluginsList;

	//导出函数表
	std::unordered_map<std::string, ExportedFuncData> exportedFuncs;

	//远程调用信息
	std::unordered_map<std::string, RemoteEngineData> remoteEngineList;

	//全局表单监听
	std::unordered_map<unsigned, FormCallbackData> formCallbacks;
};

//全局共享数据
extern GlobalDataType* engineGlobalData;