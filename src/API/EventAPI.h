#pragma once
#include "ScriptX.h"
using namespace script;

void InitEventListeners();
Local<Value> Listen(const Arguments& args);