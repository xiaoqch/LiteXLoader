#pragma once
#include <ScriptX/ScriptX.h>
#include <string>
#include "BaseAPI.h"
#include "BlockAPI.h"
#include "ItemAPI.h"
#include "EntityAPI.h"
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
#include "ScoreboardAPI.h"
#include <Engine/RemoteCall.h>
using namespace script;

class McClass
{
public:
    static Local<Value> getBDSVersion(const Arguments& args) { return GetBDSVersion(args); }

    static Local<Value> runcmd(const Arguments& args) { return Runcmd(args); }
    static Local<Value> runcmdEx(const Arguments& args) { return RuncmdEx(args); }
    static Local<Value> regPlayerCmd(const Arguments& args) { return RegisterPlayerCmd(args); }
    static Local<Value> broadcast(const Arguments& args) { return Broadcast(args); }

    static Local<Value> listen(const Arguments& args) { return Listen(args); }

    static Local<Value> getPlayer(const Arguments& args) { return GetPlayer(args); }
    static Local<Value> getOnlinePlayers(const Arguments& args) { return GetOnlinePlayers(args); }
    static Local<Value> getAllEntities(const Arguments& args) { return GetAllEntities(args); }

    static Local<Value> newItem(const Arguments& args) { return NewItem(args); }
    static Local<Value> spawnMob(const Arguments& args) { return SpawnMob(args); }
    static Local<Value> spawnItem(const Arguments& args) { return SpawnItem(args); }
    static Local<Value> explode(const Arguments& args) { return Explode(args); }

    static Local<Value> getBlock(const Arguments& args) { return GetBlock(args); }
    static Local<Value> setBlock(const Arguments& args) { return SetBlock(args); }
    static Local<Value> spawnParticle(const Arguments& args) { return SpawnParticle(args); }

    static Local<Value> newSimpleForm(const Arguments& args) { return NewSimpleForm(args); }
    static Local<Value> newCustomForm(const Arguments& args) { return NewCustomForm(args); }

    static Local<Value> regConsoleCmd(const Arguments& args) { return RegisterConsoleCmd(args); }
    static Local<Value> setMotd(const Arguments& args) { return SetMotd(args); }
    static Local<Value> sendCmdOutput(const Arguments& args) { return SendCmdOutput(args); }

    static Local<Value> newIntPos(const Arguments& args) { return NewIntPos(args); }
    static Local<Value> newFloatPos(const Arguments& args) { return NewFloatPos(args); }

    static Local<Value> getDisplayObjective(const Arguments& args) { return GetDisplayObjetive(args); }
    static Local<Value> clearDisplayObjective(const Arguments& args) { return ClearDisplayObjetive(args); }
    static Local<Value> getScoreObjective(const Arguments& args) { return GetScoreObjetive(args); }
    static Local<Value> newScoreObjective(const Arguments& args) { return NewScoreObjective(args); }
    static Local<Value> removeScoreObjective(const Arguments& args) { return RemoveScoreObjective(args); }
    static Local<Value> getAllScoreObjectives(const Arguments& args) { return GetAllScoreObjectives(args); }
};

static ClassDefine<void> McClassBuilder =
    defineClass("mc")
        .function("getBDSVersion", &McClass::getBDSVersion)
        .function("runcmd", &McClass::runcmd)
        .function("runcmdEx", &McClass::runcmdEx)
        .function("regPlayerCmd", &McClass::regPlayerCmd)
        .function("broadcast", &McClass::broadcast)
        .function("listen", &McClass::listen)
        .function("getPlayer", &McClass::getPlayer)
        .function("getOnlinePlayers", &McClass::getOnlinePlayers)
        .function("getAllEntities",McClass::getAllEntities)
        .function("newItem", &McClass::newItem)
        .function("spawnMob", &McClass::spawnMob)
        .function("spawnItem", &McClass::spawnItem)
        .function("explode", &McClass::explode)
        .function("getBlock", &McClass::getBlock)
        .function("setBlock", &McClass::setBlock)
        .function("spawnParticle", &McClass::spawnParticle)
        .function("newSimpleForm", &McClass::newSimpleForm)
        .function("newCustomForm", &McClass::newCustomForm)
        .function("regConsoleCmd", &McClass::regConsoleCmd)
        .function("setMotd", &McClass::setMotd)
        .function("sendCmdOutput", &McClass::sendCmdOutput)
        .function("newIntPos", &McClass::newIntPos)
        .function("newFloatPos", &McClass::newFloatPos)
        .function("getDisplayObjective", &McClass::getDisplayObjective)
        .function("clearDisplayObjective", &McClass::clearDisplayObjective)
        .function("getScoreObjective", &McClass::getScoreObjective)
        .function("newScoreObjective", &McClass::newScoreObjective)
        .function("removeScoreObjective", &McClass::removeScoreObjective)
        .function("getAllScoreObjectives", &McClass::getAllScoreObjectives)

        //For Compatity
        .function("getAllScoreObjective", &McClass::getAllScoreObjectives)
        .function("getDisplayObjectives", &McClass::getDisplayObjective)
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
    static Local<Value> toMD5(const Arguments& args) { return ToMD5(args); }
    static Local<Value> toSHA1(const Arguments& args) { return ToSHA1(args); }
   
};

static ClassDefine<void> DataClassBuilder =
    defineClass("data")
        .function("xuid2name", &DataClass::xuid2name)
        .function("name2xuid", &DataClass::name2xuid)
        .function("parseJson", &DataClass::parseJson)
        .function("toJson", &DataClass::toJson)
        .function("toMD5", &DataClass::toMD5)
        .function("toSHA1", &DataClass::toSHA1)

        //For Compatibility
        .function("openDB", &DataClass::openDB)
        .function("openConfig", &DataClass::openConfig)
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
    static Local<Value> newWebSocket(const Arguments& args) { return NewWebSocket(args); }
    static Local<Value> httpGet(const Arguments& args) { return HttpGet(args); }
    static Local<Value> httpPost(const Arguments& args) { return HttpPost(args); }
    static Local<Value> httpGetSync(const Arguments& args) { return HttpGetSync(args); }
};

static ClassDefine<void> NetworkClassBuilder =
    defineClass("network")
        .function("httpGet", &NetworkClass::httpGet)
        .function("httpPost", &NetworkClass::httpPost)
        .function("httpGetSync", &NetworkClass::httpGetSync)

        //For compatibility
        .function("newWebSocket", &NetworkClass::newWebSocket)
        .build();

class LxlClass
{
public:
    static Local<Value> version(const Arguments& args) { return LxlGetVersion(args); }
    static Local<Value> requireVersion(const Arguments& args) { return LxlRequireVersion(args); }
    static Local<Value> listPlugins(const Arguments& args) { return LxlListPlugins(args); }
    static Local<Value> importFunc(const Arguments& args) { return LxlImport(args); }
    static Local<Value> exportFunc(const Arguments& args) { return LxlExport(args); }
    static Local<Value> require(const Arguments& args) { return LxlRequire(args); }
    static Local<Value> eval(const Arguments& args) { return LxlEval(args); }
};

static ClassDefine<void> LxlClassBuilder =
    defineClass("lxl")
        .function("version", &LxlClass::version)
        .function("requireVersion", &LxlClass::requireVersion)
        .function("listPlugins", &LxlClass::listPlugins)
        .function("import", &LxlClass::importFunc)
        .function("export", &LxlClass::exportFunc)
        .function("require", &LxlClass::require)
        .function("eval", &LxlClass::eval)

        //For Compatibility
        .function("checkVersion", &LxlClass::requireVersion)
        .build();