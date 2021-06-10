#include <ScriptX/ScriptX.h>
#include <vector>
#include <list>
#include <string>
#include <map>

//主引擎表
extern std::list<std::shared_ptr<script::ScriptEngine>> lxlModules;

//全局表单监听表
extern std::unordered_map<int, std::pair<script::ScriptEngine*,script::Global<script::Function>> > formCallbacks;