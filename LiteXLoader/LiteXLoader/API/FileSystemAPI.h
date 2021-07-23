#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

Local<Value> FileReadFrom(const Arguments& args);
Local<Value> FileWriteTo(const Arguments& args);
Local<Value> FileWriteLine(const Arguments& args);

Local<Value> DirCreate(const Arguments& args);
Local<Value> PathCopy(const Arguments& args);
Local<Value> PathMove(const Arguments& args);
Local<Value> PathRename(const Arguments& args);
Local<Value> PathDelete(const Arguments& args);
Local<Value> PathExists(const Arguments& args);
Local<Value> GetFilesList(const Arguments& args);