#include <ScriptX/ScriptX.h>
#include <vector>
#include <list>
#include <string>
#include <map>
using namespace script;


//////////////////// Structs ////////////////////

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

//全局共享数据
struct GlobalDataType
{
	//所有插件名单
	std::vector<std::string> pluginsList;

	//导出函数表
	std::unordered_map<std::string, ExportedFuncData> exportedFuncs;

	//远程调用信息
	std::unordered_map<std::string, RemoteEngineData> remoteEngineList;
};


//////////////////// Externs ////////////////////

//全局共享数据
extern GlobalDataType* globalShareData;


//////////////////// APIs ////////////////////

void InitGlobalShareData();
void AddToGlobalPluginsList(const std::string& name);
void RemoveFromGlobalPluginsList(const std::string& name);