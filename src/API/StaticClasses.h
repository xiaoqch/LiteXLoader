#pragma once
#include "ScriptX.h"
#include <string>
#include "BaseAPI.h"
#include "BlockAPI.h"
#include "EventAPI.h"
#include "FileSystemAPI.h"
#include "LoggerAPI.h"
#include "ServerAPI.h"
#include "PlayerAPI.h"
using namespace script;

class McClass
{
public:
    static Local<Value> runcmd(const Arguments& args) { return Runcmd(args); }
    static Local<Value> runcmdEx(const Arguments& args) { return RuncmdEx(args); }
    static Local<Value> registerCmd(const Arguments& args) { return RegisterCmd(args); }

    static Local<Value> listen(const Arguments& args) { return Listen(args); }

    static Local<Value> getPlayer(const Arguments& args) { return GetPlayer(args); }
    static Local<Value> getOnlinePlayers(const Arguments& args) { return GetOnlinePlayers(args); }

    static Local<Value> getBlock(const Arguments& args) { return GetBlock(args); }
};

class SystemClass
{
public:
    static Local<Value> getTimeStr(const Arguments& args) { return GetTimeStr(args); }
    static Local<Value> getTimeObj(const Arguments& args) { return GetTimeObj(args); }
    static Local<Value> randomGuid(const Arguments& args) { return RandomGuid(args); }

    static Local<Value> cmd(const Arguments& args) { return SystemCmd(args); }
};

class FileClass
{
public:
    static Local<Value> read(const Arguments& args) { return FileReadAll(args); }
    static Local<Value> write(const Arguments& args) { return FileWriteAll(args); }
    static Local<Value> writeLine(const Arguments& args) { return FileWriteLine(args); }

    static Local<Value> createDir(const Arguments& args) { return DirCreate(args); }
    static Local<Value> copy(const Arguments& args) { return PathCopy(args); }
    static Local<Value> move(const Arguments& args) { return PathMove(args); }
    static Local<Value> rename(const Arguments& args) { return PathRename(args); }
    static Local<Value> del(const Arguments& args) { return PathDelete(args); }
    static Local<Value> exists(const Arguments& args) { return PathExists(args); }
};

class ServerClass
{
public:
    static Local<Value> setMotd(const Arguments& args) { return SetMotd(args); }
    static Local<Value> setOnlinePlayer(const Arguments& args) { return SetOnlinePlayer(args); }
};

class LoggerClass
{
public:
    static Local<Value> log(const Arguments& args) { return LoggerLog(args); }
    static Local<Value> debug(const Arguments& args) { return LoggerDebug(args); }
    static Local<Value> info(const Arguments& args) { return LoggerInfo(args); }
    static Local<Value> warn(const Arguments& args) { return LoggerWarn(args); }
    static Local<Value> error(const Arguments& args) { return LoggerError(args); }
    static Local<Value> fatal(const Arguments& args) { return LoggerFatal(args); }

    static Local<Value> setTitle(const Arguments& args) { return LoggerSetTitle(args); }
    static Local<Value> setConsole(const Arguments& args) { return LoggerSetConsole(args); }
    static Local<Value> setFile(const Arguments& args) { return LoggerSetFile(args); }
    static Local<Value> setPlayer(const Arguments& args) { return LoggerSetPlayer(args); }

    static Local<Value> setLogLevel(const Arguments& args) { return SetLogLevel(args); }
};