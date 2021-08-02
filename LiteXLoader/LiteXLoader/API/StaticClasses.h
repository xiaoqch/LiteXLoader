#pragma once
#include <ScriptX/ScriptX.h>
#include <string>
#include "BaseAPI.h"
#include "BlockAPI.h"
#include "CommandAPI.h"
#include "EventAPI.h"
#include "FileSystemAPI.h"
#include "LoggerAPI.h"
#include "ServerAPI.h"
#include "PlayerAPI.h"
#include "ScriptAPI.h"
#include "DataAPI.h"
#include "NetworkAPI.h"
#include "GuiAPI.h"
#include "LxlAPI.h"
#include "SystemAPI.h"
#include "GameSystemAPI.h"
#include <Engine/RemoteCall.h>
using namespace script;

class McClass
{
public:
    static Local<Value> runcmd(const Arguments& args) { return Runcmd(args); }
    static Local<Value> runcmdEx(const Arguments& args) { return RuncmdEx(args); }
    static Local<Value> regPlayerCmd(const Arguments& args) { return RegisterPlayerCmd(args); }
    static Local<Value> broadcast(const Arguments& args) { return Broadcast(args); }

    static Local<Value> listen(const Arguments& args) { return Listen(args); }

    static Local<Value> getPlayer(const Arguments& args) { return GetPlayer(args); }
    static Local<Value> getOnlinePlayers(const Arguments& args) { return GetOnlinePlayers(args); }

    static Local<Value> getBlock(const Arguments& args) { return GetBlock(args); }
    static Local<Value> setBlock(const Arguments& args) { return SetBlock(args); }
    static Local<Value> spawnParticle(const Arguments& args) { return SpawnParticle(args); }

    static Local<Value> newSimpleForm(const Arguments& args) { return NewSimpleForm(args); }
    static Local<Value> newCustomForm(const Arguments& args) { return NewCustomForm(args); }

    static Local<Value> regConsoleCmd(const Arguments& args) { return RegisterConsoleCmd(args); }
    static Local<Value> setMotd(const Arguments& args) { return SetMotd(args); }
    static Local<Value> setOnlinePlayer(const Arguments& args) { return SetOnlinePlayer(args); }
    static Local<Value> sendCmdOutput(const Arguments& args) { return SendCmdOutput(args); }

    static Local<Value> newIntPos(const Arguments& args) { return NewIntPos(args); }
    static Local<Value> newFloatPos(const Arguments& args) { return NewFloatPos(args); }

    static Local<Value> newScoreObjective(const Arguments& args) { return NewScoreObjective(args); }
    static Local<Value> removeScoreObjective(const Arguments& args) { return RemoveScoreObjective(args); }
    static Local<Value> listAllScoreObjective(const Arguments& args) { return ListAllScoreObjective(args); }
};

static ClassDefine<void> McClassBuilder =
    defineClass("mc")
        .function("runcmd", &McClass::runcmd)
        .function("runcmdEx", &McClass::runcmdEx)
        .function("regPlayerCmd", &McClass::regPlayerCmd)
        .function("broadcast", &McClass::broadcast)
        .function("listen", &McClass::listen)
        .function("getPlayer", &McClass::getPlayer)
        .function("getOnlinePlayers", &McClass::getOnlinePlayers)
        .function("getBlock", &McClass::getBlock)
        .function("setBlock", &McClass::setBlock)
        .function("spawnParticle", &McClass::spawnParticle)
        .function("newSimpleForm", &McClass::newSimpleForm)
        .function("newCustomForm", &McClass::newCustomForm)
        .function("regConsoleCmd", &McClass::regConsoleCmd)
        .function("setMotd", &McClass::setMotd)
        .function("setOnlinePlayer", &McClass::setOnlinePlayer)
        .function("sendCmdOutput", &McClass::sendCmdOutput)
        .function("newIntPos", &McClass::newIntPos)
        .function("newFloatPos", &McClass::newFloatPos)
        .function("newScoreObjective", &McClass::newScoreObjective)
        .function("removeScoreObjective", &McClass::removeScoreObjective)
        .function("listAllScoreObjective", &McClass::listAllScoreObjective)
        .function("crash", CrashBDS)
        .build();


class SystemClass
{
public:
    static Local<Value> getTimeStr(const Arguments& args) { return GetTimeStr(args); }
    static Local<Value> getTimeObj(const Arguments& args) { return GetTimeObj(args); }
    static Local<Value> randomGuid(const Arguments& args) { return RandomGuid(args); }

    static Local<Value> cmd(const Arguments& args) { return SystemCmd(args); }
    static Local<Value> newProcess(const Arguments& args) { return SystemNewProcess(args); }
};

static ClassDefine<void> SystemClassBuilder =
    defineClass("system")
        .function("getTimeStr", &SystemClass::getTimeStr)
        .function("getTimeObj", &SystemClass::getTimeObj)
        .function("randomGuid", &SystemClass::randomGuid)
        .function("cmd", &SystemClass::cmd)
        .function("newProcess", &SystemClass::newProcess)
        .build();


class FileClass
{
public:
    static Local<Value> readFrom(const Arguments& args) { return FileReadFrom(args); }
    static Local<Value> writeTo(const Arguments& args) { return FileWriteTo(args); }
    static Local<Value> writeLine(const Arguments& args) { return FileWriteLine(args); }

    static Local<Value> createDir(const Arguments& args) { return DirCreate(args); }
    static Local<Value> copy(const Arguments& args) { return PathCopy(args); }
    static Local<Value> move(const Arguments& args) { return PathMove(args); }
    static Local<Value> rename(const Arguments& args) { return PathRename(args); }
    static Local<Value> del(const Arguments& args) { return PathDelete(args); }
    static Local<Value> exists(const Arguments& args) { return PathExists(args); }
    static Local<Value> getFilesList(const Arguments& args) { return GetFilesList(args); }
};

static ClassDefine<void> FileClassBuilder =
    defineClass("file")
        .function("readFrom", &FileClass::readFrom)
        .function("writeTo", &FileClass::writeTo)
        .function("writeLine", &FileClass::writeLine)

        .function("createDir", &FileClass::createDir)
        .function("mkdir", &FileClass::createDir)
        .function("copy", &FileClass::copy)
        .function("move", &FileClass::move)
        .function("rename", &FileClass::rename)
        .function("delete", &FileClass::del)
        .function("exists", &FileClass::exists)
        .function("getFilesList", &FileClass::getFilesList)
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

static ClassDefine<void> LoggerClassBuilder =
    defineClass("logger")
        .function("log", &LoggerClass::log)
        .function("debug", &LoggerClass::debug)
        .function("info", &LoggerClass::info)
        .function("warn", &LoggerClass::warn)
        .function("warning", & LoggerClass::warn)
        .function("error", &LoggerClass::error)
        .function("fatal", &LoggerClass::fatal)

        .function("setTitle", &LoggerClass::setTitle)
        .function("setConsole", &LoggerClass::setConsole)
        .function("setFile", &LoggerClass::setFile)
        .function("setPlayer", &LoggerClass::setPlayer)
        .function("setLogLevel", &LoggerClass::setLogLevel)
        .build();

class DataClass
{
public:
    static Local<Value> openConfig(const Arguments& args) { return OpenConfig(args); }
    static Local<Value> openDB(const Arguments& args) { return OpenDB(args); }

    static Local<Value> xuid2name(const Arguments& args) { return Xuid2Name(args); }
    static Local<Value> name2xuid(const Arguments& args) { return Name2Xuid(args); }

    static Local<Value> parseJson(const Arguments& args) { return ParseJson(args); }
    static Local<Value> toJson(const Arguments& args) { return ToJson(args); }
};

static ClassDefine<void> DataClassBuilder =
    defineClass("data")
        .function("openConfig", &DataClass::openConfig)
        .function("openDB", &DataClass::openDB)
        .function("xuid2name", &DataClass::xuid2name)
        .function("name2xuid", &DataClass::name2xuid)
        .function("parseJson", &DataClass::parseJson)
        .function("toJson", &DataClass::toJson)
        .build();

class MoneyClass
{
public:
    static Local<Value> set(const Arguments& args) { return MoneySet(args); }
    static Local<Value> get(const Arguments& args) { return MoneyGet(args); }
    static Local<Value> add(const Arguments& args) { return MoneyAdd(args); }
    static Local<Value> reduce(const Arguments& args) { return MoneyReduce(args); }
    static Local<Value> trans(const Arguments& args) { return MoneyTrans(args); }
    static Local<Value> getHistory(const Arguments& args) { return MoneyGetHintory(args); }
    static Local<Value> clearHistory(const Arguments& args) { return MoneyClearHistory(args); }
};

static ClassDefine<void> MoneyClassBuilder =
    defineClass("money")
        .function("set", &MoneyClass::set)
        .function("get", &MoneyClass::get)
        .function("add", &MoneyClass::add)
        .function("reduce", &MoneyClass::reduce)
        .function("trans", &MoneyClass::trans)
        .function("getHistory", &MoneyClass::getHistory)
        .function("clearHistory", &MoneyClass::clearHistory)
        .build();

class NetworkClass
{
public:
    static Local<Value> httpGet(const Arguments& args) { return HttpGet(args); }
    static Local<Value> httpPost(const Arguments& args) { return HttpPost(args); }
};

static ClassDefine<void> NetworkClassBuilder =
    defineClass("network")
        .function("httpGet", &NetworkClass::httpGet)
        .function("httpPost", &NetworkClass::httpPost)
        .build();

class LxlClass
{
public:
    static Local<Value> version(const Arguments& args) { return LxlGetVersion(args); }
    static Local<Value> checkVersion(const Arguments& args) { return LxlCheckVersion(args); }
    static Local<Value> listPlugins(const Arguments& args) { return LxlListPlugins(args); }
    static Local<Value> importFunc(const Arguments& args) { return LxlImport(args); }
    static Local<Value> exportFunc(const Arguments& args) { return LxlExport(args); }
    static Local<Value> require(const Arguments& args) { return LxlRequire(args); }
    static Local<Value> eval(const Arguments& args) { return LxlEval(args); }
};

static ClassDefine<void> LxlClassBuilder =
    defineClass("lxl")
        .function("version", &LxlClass::version)
        .function("checkVersion", &LxlClass::checkVersion)
        .function("listPlugins", &LxlClass::listPlugins)
        .function("import", &LxlClass::importFunc)
        .function("export", &LxlClass::exportFunc)
        .function("require", &LxlClass::require)
        .function("eval", &LxlClass::eval)
        .build();