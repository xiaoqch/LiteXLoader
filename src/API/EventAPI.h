#pragma once
#include "ScriptX.h"

void InitEventAPIs();
script::Local<script::Value> AddEventListener(const script::Arguments& args);