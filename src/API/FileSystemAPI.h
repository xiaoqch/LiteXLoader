#pragma once
#include "ScriptX.h"
using namespace script;

Local<Value> FileReadAll(const Arguments& args);
Local<Value> FileWriteAll(const Arguments& args);
Local<Value> FileWriteLine(const Arguments& args);

Local<Value> DirCreate(const Arguments& args);
Local<Value> PathCopy(const Arguments& args);
Local<Value> PathMove(const Arguments& args);
Local<Value> PathRename(const Arguments& args);
Local<Value> PathDelete(const Arguments& args);
Local<Value> PathExists(const Arguments& args);

Local<Value> SystemCmd(const Arguments& args);