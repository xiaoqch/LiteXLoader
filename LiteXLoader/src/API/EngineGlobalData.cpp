#include "EngineGlobalData.h"
#include <ScriptX/ScriptX.h>
#include <string>
#include <vector>
#include <map>
using namespace std;
using namespace script;

//全局表单监听
EngineGlobalData_Type* engineGlobalData;

void InitEngineGlobalData()
{
	engineGlobalData = new EngineGlobalData_Type;
}