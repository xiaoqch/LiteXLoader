#pragma once
#include "ScriptX.h"
using namespace script;

void RegisterEventListeners();
Local<Value> Listen(const Arguments& args);