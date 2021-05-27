#pragma once
#include "ScriptX.h"
using namespace script;

Local<Value> SendSimpleForm(const Arguments& args);
Local<Value> SendModelForm(const Arguments& args);
Local<Value> GiveUpForm(const Arguments& args);
Local<Value> CreateForm(const Arguments& args);