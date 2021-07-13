#pragma once
#include "APIhelp.h"

bool InitRemoteCallSystem();

//////////////////// APIs ////////////////////

Local<Value> LxlExport(const Arguments& args);
Local<Value> LxlImport(const Arguments& args);