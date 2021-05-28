#pragma once
#include "ScriptX.h"
using namespace script;

Local<Value> HttpRequestSync(const Arguments& args);
Local<Value> HttpRequestAsync(const Arguments& args);