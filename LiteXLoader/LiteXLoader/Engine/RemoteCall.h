#pragma once
#include <ScriptX/ScriptX.h>
#include <string>

//////////////////// Funcs ////////////////////

bool LxlExportFunc(ScriptEngine* engine, const Local<Function>& func, const string& exportName);
bool LxlRemoveAllExportedFuncs(ScriptEngine* engine);

class ModuleMessage;
void RemoteCallCallback(ModuleMessage& msg);
void RemoteCallReturnCallback(ModuleMessage& msg);

//////////////////// APIs ////////////////////

Local<Value> LxlExport(const Arguments& args);
Local<Value> LxlImport(const Arguments& args);