#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

Local<Value> HttpGet(const Arguments& args);
Local<Value> HttpPost(const Arguments& args);
Local<Value> HttpGetSync(const Arguments& args);