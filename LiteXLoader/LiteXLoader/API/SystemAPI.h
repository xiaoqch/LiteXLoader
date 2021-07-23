#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

Local<Value> SystemCmd(const Arguments& args);
Local<Value> SystemNewProcess(const Arguments& args);