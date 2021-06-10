#include "EngineGlobalData.h"
#include <ScriptX/ScriptX.h>
#include <string>
#include <vector>
#include <map>
using namespace std;
using namespace script;

std::unordered_map<int, std::pair<ScriptEngine*,Global<Function>> > formCallbacks;