#include "APIHelp.h"
#include "EngineGlobalData.h"
#include "RemoteCall.h"
#include <ScriptX/ScriptX.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <Configs.h>
#include "APIHelp.h"
using namespace std;
using namespace script;

//全局共享数据
GlobalDataType* engineGlobalData;

//DLL本地共享数据
LocalDataType* engineLocalData;

//命令延迟注册队列
std::vector<RegCmdQueue> toRegCmdQueue;

void InitEngineGlobalData()
{
	srand(clock());

	engineLocalData = new LocalDataType;

	HANDLE hGlobalData = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(GlobalDataType), (LXL_GLOBAL_DATA_NAME + to_wstring(GetCurrentProcessId())).c_str());
	if (hGlobalData == NULL)
	{
		ERROR(_TRS("init.fileMapping.fail"));
		engineLocalData->isFirstInstance = true;
		return;
	}

	LPVOID address = MapViewOfFile(hGlobalData, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (address == NULL)
	{
		ERROR(_TRS("init.mapFile.fail"));
		engineLocalData->isFirstInstance = true;
		return;
	}

	if (GetLastError() != ERROR_ALREADY_EXISTS)
	{
		//First Time
		engineLocalData->isFirstInstance = true;
		engineGlobalData = new (address) GlobalDataType;
	}
	else
	{
		//Existing
		engineLocalData->isFirstInstance = false;
		engineGlobalData = (GlobalDataType*)address;
	}

	InitRemoteCallSystem();
}

void AddToGlobalPluginsList(const std::string& name)
{
	engineGlobalData->pluginsList.push_back(name);
}

void RemoveFromGlobalPluginsList(const std::string& name)
{
	for (int i = 0; i < engineGlobalData->pluginsList.size(); ++i)
	{
		if (engineGlobalData->pluginsList[i] == name)
		{
			engineGlobalData->pluginsList.erase(engineGlobalData->pluginsList.begin() + i);
			break;
		}
	}
}