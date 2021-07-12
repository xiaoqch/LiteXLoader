#include <ScriptX/ScriptX.h>
#include <vector>
#include <list>
#include <string>
#include <map>

//主引擎表
extern std::list<std::shared_ptr<script::ScriptEngine>> lxlModules;

void InitEngineGlobalData(bool* isFirstInstance);
struct FormCallbackData
{
	script::ScriptEngine* engine;
	script::Global<script::Function> func;
};
struct EngineGlobalData_Type
{
	//全局表单监听
	std::unordered_map<unsigned, FormCallbackData> formCallbacks;
};
extern EngineGlobalData_Type* engineGlobalData;