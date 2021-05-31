#include "ScriptX.h"
#include <map>

//主引擎表
extern std::list<std::shared_ptr<ScriptEngine>> lxlModules;

//全局表单监听表
extern std::unordered_map<int, std::pair<script::ScriptEngine*,script::Global<script::Function>> > formCallbacks;