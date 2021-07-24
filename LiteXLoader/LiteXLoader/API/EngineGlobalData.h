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

//命令延迟注册队列
struct RegCmdQueue
{
	std::string cmd;
	std::string describe;
	int level;
};

//命令回调信息结构体
struct CmdCallbackData
{
	ScriptEngine* fromEngine;
	int perm;
	Global<Function> func;
};

//命令回调map排序
static struct EngineOwnData_MapCmp
{
	bool operator() (std::string const& a, std::string const& b) const
	{
		if (a.size() != b.size())
			return a.size() > b.size();
		else
			return a > b;
	}
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

//DLL本地共享数据
struct LocalDataType
{
	//是否是第一个LXL实例（最底层Hook）
	bool isFirstInstance = true;

	//事件回调拦截情况（层次传递设计）
	bool isPassToBDS = true;

	//玩家命令回调
	std::map<std::string, CmdCallbackData, EngineOwnData_MapCmp> playerCmdCallbacks;

	//控制台命令回调
	std::map<std::string, CmdCallbackData, EngineOwnData_MapCmp> consoleCmdCallbacks;
};


//////////////////// Externs ////////////////////

//本地引擎表
extern std::vector<ScriptEngine*> lxlModules;

//全局共享数据
extern GlobalDataType* engineGlobalData;

//DLL本地共享数据
extern LocalDataType* engineLocalData;

//命令延迟注册队列
extern std::vector<RegCmdQueue> toRegCmdQueue;


//////////////////// APIs ////////////////////

void InitEngineGlobalData();
void AddToGlobalPluginsList(const std::string& name);
void RemoveFromGlobalPluginsList(const std::string& name);