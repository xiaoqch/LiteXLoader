#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

//////////////////// Funcs ////////////////////

void InitEventListeners();
bool LxlAddEventListener(ScriptEngine* engine, const std::string& eventName, const Local<Function>& func);
bool LxlRemoveAllEventListeners(ScriptEngine* engine);
bool LxlRecallOnServerStarted(ScriptEngine* engine);


//////////////////// APIs ////////////////////

Local<Value> Listen(const Arguments& args);