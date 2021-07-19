#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

//////////////////// Classes ////////////////////

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

#define LOGGER_CHECK_DEBUG(level) (level >= 5)
#define LOGGER_CHECK_INFO(level) (level >= 4)
#define LOGGER_CHECK_WARN(level) (level >= 3)
#define LOGGER_CHECK_ERROR(level) (level >= 2)
#define LOGGER_CHECK_FATAL(level) (level >= 1)