#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

//////////////////// Init ////////////////////

void InitDynamicCallSystem();

//////////////////// APIs ////////////////////

Local<Value> Hook(const Arguments& args);
Local<Value> GetSymbol(const Arguments& args);


//////////////////// Class Define ////////////////////

class NativeClass
{
public:
    static Local<Value> getSymbol(const Arguments& args) { return GetSymbol(args); }
    static Local<Value> hook(const Arguments& args) { return Hook(args); }
};