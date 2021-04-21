#pragma once
#include "ScriptX.h"

script::Local<script::Value> RunCmd(const script::Arguments& args);
script::Local<script::Value> RunCmdAs(const script::Arguments& args);

script::Local<script::Value> SendText(const script::Arguments& args);