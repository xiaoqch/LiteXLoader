#pragma once
#include "ScriptX.h"
#include <functional>

void InitEventAPIs();
script::Local<script::Value> AddEventListener(const script::Arguments& args);