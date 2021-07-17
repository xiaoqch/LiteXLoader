#include "EngineGlobalData.h"
#include "RemoteCall.h"
#include <ScriptX/ScriptX.h>
#include <string>
#include <vector>
#include <map>
#include <Configs.h>
#include "APIhelp.h"
using namespace std;
using namespace script;

//全局共享数据
GlobalDataType* engineGlobalData;

void InitEngineGlobalData(bool *isFirstInstance)
{
	HANDLE hGlobalData = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(GlobalDataType), (LXL_GLOBAL_DATA_NAME + to_wstring(GetCurrentProcessId())).c_str());
	if (hGlobalData == NULL)
	{
		ERROR(_TRS("init.fileMapping.fail"));
		*isFirstInstance = true;
		return;
	}

	LPVOID address = MapViewOfFile(hGlobalData, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (address == NULL)
	{
		ERROR(_TRS("init.mapFile.fail"));
		*isFirstInstance = true;
		return;
	}

	if (GetLastError() != ERROR_ALREADY_EXISTS)
	{
		//First Time
		*isFirstInstance = true;
		engineGlobalData = new (address) GlobalDataType;
	}
	else
	{
		//Existing
		*isFirstInstance = false;
		engineGlobalData = (GlobalDataType*)address;
	}

	InitRemoteCallSystem();
}