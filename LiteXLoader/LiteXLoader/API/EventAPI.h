#pragma once
#include <ScriptX/ScriptX.h>
#include <Kernel/Data.h>
using namespace script;

//////////////////// Funcs ////////////////////

void InitEventListeners();
bool LxlAddEventListener(ScriptEngine* engine, const std::string& eventName, const Local<Function>& func);
bool LxlRemoveAllEventListeners(ScriptEngine* engine);
bool LxlCallEventsOnHotLoad(ScriptEngine* engine);
bool LxlCallEventsOnHotUnload(ScriptEngine* engine);

//////////////////// Callback ////////////////////

bool MoneyEventCallback(LLMoneyEvent type, xuid_t from, xuid_t to, money_t value);