#pragma once
#include <ScriptX/ScriptX.h>
#include <string>
#include <vector>
using namespace script;

//基础库 & 依赖库
extern std::vector<std::string> depends;

//前置声明
extern std::list<std::shared_ptr<ScriptEngine>> lxlModules;
extern void BindAPIs(std::shared_ptr<ScriptEngine> engine);