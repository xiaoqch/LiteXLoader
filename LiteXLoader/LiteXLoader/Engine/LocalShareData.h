#pragma once
#include <ScriptX/ScriptX.h>
#include <map>
#include <string>
using namespace script;


//////////////////// Structs ////////////////////

//命令回调信息结构体
struct CmdCallbackData
{
	ScriptEngine* fromEngine;
	int perm;
	Global<Function> func;
};

//命令延迟注册队列
struct RegCmdQueue
{
	std::string cmd;
	std::string describe;
	int level;
};

//命令回调map排序
static struct CmdCallbackMapCmp
{
	bool operator() (std::string const& a, std::string const& b) const
	{
		if (a.size() != b.size())
			return a.size() > b.size();
		else
			return a > b;
	}
};

//设备信息记录
struct DeviceInfoType
{
	std::string DeviceId;
	int DeviceOS;
};

//DLL本地共享数据
struct LocalDataType
{
	//是否是第一个LXL实例（最底层Hook）
	bool isFirstInstance = true;

	//事件回调拦截情况（层次传递设计）
	bool isPassToBDS = true;

	//玩家命令回调
	std::map<std::string, CmdCallbackData, CmdCallbackMapCmp> playerCmdCallbacks;

	//控制台命令回调
	std::map<std::string, CmdCallbackData, CmdCallbackMapCmp> consoleCmdCallbacks;

	//设备信息记录
	std::unordered_map<uintptr_t , DeviceInfoType> deviceInfoRecord;
};


//////////////////// Externs ////////////////////

//DLL本地共享数据
extern LocalDataType* localShareData;

//命令延迟注册队列
extern std::vector<RegCmdQueue> toRegCmdQueue;

//本地引擎表
extern std::vector<ScriptEngine*> lxlModules;

//////////////////// APIs ////////////////////

void InitLocalShareData();