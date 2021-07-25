#pragma once
#include <ScriptX/ScriptX.h>
#include <string>

//////////////////// Funcs ////////////////////

bool InitRemoteCallSystem();
bool LxlExportFunc(ScriptEngine* engine, const Local<Function>& func, const string& exportName);
bool LxlRemoveAllExportedFuncs(ScriptEngine* engine);

//////////////////// APIs ////////////////////

Local<Value> LxlExport(const Arguments& args);
Local<Value> LxlImport(const Arguments& args);