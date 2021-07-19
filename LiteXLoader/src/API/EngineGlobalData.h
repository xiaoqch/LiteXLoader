#include <ScriptX/ScriptX.h>
#include <vector>
#include <list>
#include <string>
#include <map>
using namespace script;

void InitEngineGlobalData(bool* isFirstInstance);

//主引擎表
extern std::vector<ScriptEngine*> lxlModules;

//导出函数表
struct ExportedFuncData
{
	std::string fromEngineType;
	ScriptEngine* engine;
	Global<Function> func;	/////////////// 可能有问题，不崩就不改
};

//远程调用信息
struct RemoteEngineData
{
	unsigned threadId;
};


//表单回调信息
struct FormCallbackKey
{
	std::string fromEngineType;
	unsigned formId;
};

bool inline operator<(const FormCallbackKey& a, const FormCallbackKey& b)
{
	return a.fromEngineType == b.fromEngineType ? a.formId < b.formId : a.fromEngineType < b.fromEngineType;
}

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
	std::map<FormCallbackKey, FormCallbackData> formCallbacks;
};

//命令延迟注册队列
struct RegCmdQueue
{
	std::string cmd;
	std::string describe;
	int level;
};
extern std::vector<RegCmdQueue> toRegCmdQueue;

//全局共享数据
extern GlobalDataType* engineGlobalData;

void AddToGlobalPluginsList(const std::string& name);
void RemoveFromGlobalPluginsList(const std::string& name);