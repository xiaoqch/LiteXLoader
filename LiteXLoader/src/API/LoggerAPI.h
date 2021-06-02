#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

Local<Value> LoggerLog(const Arguments& args);
Local<Value> LoggerDebug(const Arguments& args);
Local<Value> LoggerInfo(const Arguments& args);
Local<Value> LoggerWarn(const Arguments& args);
Local<Value> LoggerError(const Arguments& args);
Local<Value> LoggerFatal(const Arguments& args);

Local<Value> LoggerSetTitle(const Arguments& args);
Local<Value> LoggerSetConsole(const Arguments& args);
Local<Value> LoggerSetFile(const Arguments& args);
Local<Value> LoggerSetPlayer(const Arguments& args);

Local<Value> SetLogLevel(const Arguments& args);