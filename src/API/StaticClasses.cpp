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
#include "DbAPI.h"
#include "NetworkAPI.h"
#include "GuiAPI.h"
using namespace script;

class McClass
{
public:
    static Local<Value> runcmd(const Arguments& args) { return Runcmd(args); }
    static Local<Value> runcmdEx(const Arguments& args) { return RuncmdEx(args); }
    static Local<Value> registerCmd(const Arguments& args) { return RegisterPlayerCmd(args); }

    static Local<Value> listen(const Arguments& args) { return Listen(args); }

    static Local<Value> getPlayer(const Arguments& args) { return GetPlayer(args); }
    static Local<Value> getOnlinePlayers(const Arguments& args) { return GetOnlinePlayers(args); }
    static Local<Value> getBlock(const Arguments& args) { return GetBlock(args); }

    static Local<Value> newForm(const Arguments& args) { return NewForm(args); }
    static Local<Value> cancelForm(const Arguments& args) { return CancelForm(args); }
};
ClassDefine<void> McClassBuilder =
    defineClass("mc")
        .function("runcmd", &McClass::runcmd)
        .function("runcmdEx", &McClass::runcmdEx)
        .function("registerCmd", &McClass::registerCmd)
        .function("listen", &McClass::listen)
        .function("getPlayer", &McClass::getPlayer)
        .function("getOnlinePlayers", &McClass::getOnlinePlayers)
        .function("getBlock", &McClass::getBlock)
        .function("newForm", &McClass::newForm)
        .function("cancelForm", &McClass::cancelForm)
        .build();


class SystemClass
{
public:
    static Local<Value> getTimeStr(const Arguments& args) { return GetTimeStr(args); }
    static Local<Value> getTimeObj(const Arguments& args) { return GetTimeObj(args); }
    static Local<Value> randomGuid(const Arguments& args) { return RandomGuid(args); }

    static Local<Value> cmd(const Arguments& args) { return SystemCmd(args); }
};

ClassDefine<void> SystemClassBuilder =
    defineClass("system")
        .function("getTimeStr", &SystemClass::getTimeStr)
        .function("getTimeObj", &SystemClass::getTimeObj)
        .function("randomGuid", &SystemClass::randomGuid)
        .function("cmd", &SystemClass::cmd)
        .build();


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

ClassDefine<void> FileClassBuilder =
    defineClass("file")
        .function("read", &FileClass::read)
        .function("write", &FileClass::write)
        .function("writeLine", &FileClass::writeLine)

        .function("createDir", &FileClass::createDir)
        .function("copy", &FileClass::copy)
        .function("move", &FileClass::move)
        .function("rename", &FileClass::rename)
        .function("delete", &FileClass::del)
        .function("exists", &FileClass::exists)
        .build();


class ServerClass
{
public:
    static Local<Value> registerCmd(const Arguments& args) { return RegisterConsoleCmd(args); }

    static Local<Value> setMotd(const Arguments& args) { return SetMotd(args); }
    static Local<Value> setOnlinePlayer(const Arguments& args) { return SetOnlinePlayer(args); }
};

ClassDefine<void> ServerClassBuilder =
    defineClass("server")
        .function("registerCmd", &ServerClass::registerCmd)
        .function("setMotd", &ServerClass::setMotd)
        .function("setOnlinePlayer", &ServerClass::setOnlinePlayer)
        .build();


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

ClassDefine<void> LoggerClassBuilder =
    defineClass("logger")
        .function("log", &LoggerClass::log)
        .function("debug", &LoggerClass::debug)
        .function("info", &LoggerClass::info)
        .function("warn", &LoggerClass::warn)
        .function("error", &LoggerClass::error)
        .function("fatal", &LoggerClass::fatal)

        .function("setTitle", &LoggerClass::setTitle)
        .function("setConsole", &LoggerClass::setConsole)
        .function("setFile", &LoggerClass::setFile)
        .function("setPlayer", &LoggerClass::setPlayer)
        .function("setLogLevel", &LoggerClass::setLogLevel)
        .build();

class ConfClass
{
public:
    static Local<Value> init(const Arguments& args) { return ConfInit(args); }

    static Local<Value> set(const Arguments& args) { return ConfJsonSet(args); }
    static Local<Value> get(const Arguments& args) { return ConfJsonGet(args); }
    static Local<Value> del(const Arguments& args) { return ConfJsonDelete(args); }

    static Local<Value> setKey(const Arguments& args) { return ConfIniSet(args); }
    static Local<Value> getInt(const Arguments& args) { return ConfIniGetInt(args); }
    static Local<Value> getStr(const Arguments& args) { return ConfIniGetStr(args); }
    static Local<Value> getFloat(const Arguments& args) { return ConfIniGetFloat(args); }
    static Local<Value> getBool(const Arguments& args) { return ConfIniGetBool(args); }
    static Local<Value> deleteKey(const Arguments& args) { return ConfIniDeleteKey(args); }
    static Local<Value> deleteSec(const Arguments& args) { return ConfIniDeleteSec(args); }

    static Local<Value> reload(const Arguments& args) { return ConfReload(args); }
    static Local<Value> getPath(const Arguments& args) { return ConfGetPath(args); }
    static Local<Value> read(const Arguments& args) { return ConfRead(args); }
    static Local<Value> write(const Arguments& args) { return ConfWrite(args); }

    static Local<Value> openDB(const Arguments& args) { return OpenDB(args); }
};

ClassDefine<void> ConfClassBuilder =
    defineClass("conf")
        .function("init", &ConfClass::init)

        .function("set", &ConfClass::set)
        .function("get", &ConfClass::get)
        .function("delete", &ConfClass::del)

        .function("setKey", &ConfClass::setKey)
        .function("getInt", &ConfClass::getInt)
        .function("getStr", &ConfClass::getStr)
        .function("getFloat", &ConfClass::getFloat)
        .function("getBool", &ConfClass::getBool)
        .function("deleteKey", &ConfClass::deleteKey)
        .function("deleteSec", &ConfClass::deleteSec)

        .function("reload", &ConfClass::reload)
        .function("getPath", &ConfClass::getPath)
        .function("read", &ConfClass::read)
        .function("write", &ConfClass::write)

        .function("openDB", &ConfClass::openDB)
        .build();

class NetworkClass
{
public:
    static Local<Value> requestSync(const Arguments& args) { return HttpRequestSync(args); }
    static Local<Value> request(const Arguments& args) { return HttpRequestAsync(args); }
};

ClassDefine<void> NetworkClassBuilder =
    defineClass("network")
        .function("requestSync", &NetworkClass::requestSync)
        .function("request", &NetworkClass::request)
        .build();