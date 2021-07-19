#include "CommandAPI.h"
#include "APIHelp.h"
#include "PlayerAPI.h"
#include "EngineGlobalData.h"
#include "EngineOwnData.h"
#include "LoaderHelper.h"
#include <Kernel/Base.h>
#include <Kernel/Global.h>
#include <filesystem>
#include <Configs.h>
#include <vector>
#include <string>
using namespace std;

//////////////////// APIs ////////////////////

Local<Value> Runcmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        return Boolean::newBoolean(Raw_Runcmd(args[0].asString().toString()));
    }
    CATCH("Fail in RunCmd!")
}

Local<Value> RuncmdEx(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        std::pair<bool, string> result = Raw_RuncmdEx(args[0].asString().toString());
        Local<Object> resObj = Object::newObject();
        resObj.set("result", result.first);
        resObj.set("output", result.second);
        return resObj;
    }
    CATCH("Fail in RunCmdEx!")
}

Local<Value> RegisterPlayerCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 3)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    CHECK_ARG_TYPE(args[1], ValueKind::kString)
    CHECK_ARG_TYPE(args[2], ValueKind::kFunction)

    if (args.size() >= 4)
        CHECK_ARG_TYPE(args[3], ValueKind::kNumber)

        try {
            string cmd = args[0].asString().toString();
            string describe = args[1].asString().toString();
            int level = 0;

            if (args.size() >= 4)
            {
                int newLevel = args[3].asNumber().toInt32();
                if (newLevel >= 0 && newLevel <= 3)
                    level = newLevel;
            }

            if (cmd[0] == '/')
                cmd = cmd.erase(0, 1);
            (ENGINE_OWN_DATA()->playerCmdCallbacks)[cmd] = args[2].asFunction();

            //延迟注册
            if(isCmdRegisterEnabled)
                Raw_RegisterCmd(cmd, describe, level);
            else
                toRegCmdQueue.push_back({ cmd, describe, level });

            return Boolean::newBoolean(true);
        }
    CATCH("Fail in RegisterPlayerCmd!")
}

Local<Value> RegisterConsoleCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 3)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    CHECK_ARG_TYPE(args[1], ValueKind::kString)
    CHECK_ARG_TYPE(args[2], ValueKind::kFunction)

    try {
        string cmd = args[0].asString().toString();
        string describe = args[1].asString().toString();

        if (cmd[0] == '/')
            cmd = cmd.erase(0, 1);
        (ENGINE_OWN_DATA()->consoleCmdCallbacks)[cmd] = args[2].asFunction();

        //延迟注册
        if (isCmdRegisterEnabled)
            Raw_RegisterCmd(cmd, describe, 4);
        else
            toRegCmdQueue.push_back({ cmd, describe, 4 });

        return Boolean::newBoolean(true);
    }
    CATCH("Fail in RegisterConsoleCmd!")
}

Local<Value> SendCmdOutput(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        return Boolean::newBoolean(Raw_SendCmdOutput(args[0].toStr()));
    }
    CATCH("Fail in SendCmdOutput!")
}


//////////////////// LXL Event Callbacks ////////////////////

// Helper
vector<string> SplitCmdParas(const string& paras)
{
    if (paras.empty())
        return vector<string>();

    vector<string> res;
    string now, strInQuote = "";
    istringstream strIn(paras);
    while (strIn >> now)
    {
        if (!strInQuote.empty())
        {
            strInQuote = strInQuote + " " + now;
            if (now.back() == '\"')
            {
                strInQuote.pop_back();
                res.push_back(strInQuote.erase(0, 1));
                strInQuote = "";
            }
        }
        else
        {
            if (now.front() == '\"')
                strInQuote = now;
            else
                res.push_back(now);
        }
    }
    if (!strInQuote.empty())
    {
        istringstream leftIn(strInQuote);
        while (leftIn >> now)
            res.push_back(now);
    }
    return res;
}
// Helper


void RegisterBuiltinCmds()
{
    //调试引擎
    Raw_RegisterCmd(LXL_DEBUG_CMD, "LXL " + string(LXL_SCRIPT_LANG_TYPE) + " Engine Real-time Debugging", 4);
    
    //热管理
    Raw_RegisterCmd(LXL_HOT_LIST, "List current loaded LXL plugins", 4);
    Raw_RegisterCmd(LXL_HOT_LOAD, "Load a new LXL plugin", 4);
    Raw_RegisterCmd(LXL_HOT_UNLOAD, "Unload an existing LXL plugin", 4);
    Raw_RegisterCmd(LXL_HOT_RELOAD, "Reload an existing LXL plugin / all LXL plugins", 4);

    INFO("Builtin Cmds Registered.");
}

void ProcessRegCmdQueue()
{
    for (auto& cmdData : toRegCmdQueue)
    {
        Raw_RegisterCmd(cmdData.cmd, cmdData.describe, cmdData.level);
    }
    toRegCmdQueue.clear();
}

bool ProcessDebugEngine(const string& cmd)
{
#define OUTPUT_DEBUG_SIGN() std::cout << "LiteXLoader-" << LXL_SCRIPT_LANG_TYPE << ">" << std::flush
    extern bool globalDebug;
    extern ScriptEngine *debugEngine;

    if (cmd == LXL_DEBUG_CMD)
    {
        if (globalDebug)
        {
            //EndDebug
            INFO("Debug mode ended");
            globalDebug = false;
        }
        else
        {
            //StartDebug
            INFO("Debug mode begin");
            globalDebug = true;
            OUTPUT_DEBUG_SIGN();
        }
        return false;
    }
    if (globalDebug)
    {
        EngineScope enter(debugEngine);
        try
        {
            if (cmd == "stop")
            {
                WARN("请先退出Debug实时调试模式再使用stop！");
            }
            else
            {
                auto result = debugEngine->eval(cmd);
                PrintValue(std::cout, result);
                cout << endl;
                OUTPUT_DEBUG_SIGN();
            }
        }
        catch (Exception& e)
        {
            ERRPRINT(e);
            OUTPUT_DEBUG_SIGN();
        }
        return false;
    }
    return true;
}

bool ProcessHotManagement(const std::string& cmd)
{
    auto cmdList = SplitCmdParas(cmd);
    if (cmdList[0] != LXL_HOT_MANAGE_PREFIX)
        return true;

    if (cmd.find(LXL_HOT_LIST) == 0)
    {
        //list
        auto list = LxlListLocalAllPlugins();
        for (auto& name : list)
            PRINT(name);
    }
    else if (cmd.find(LXL_HOT_LOAD) == 0 && cmdList.size() == 3)
    {
        //load
        if (!filesystem::exists(cmdList[2]))
            ERROR("Plugin no found!");
        LxlLoadPlugin(cmdList[2]);
    }
    else if (cmd.find(LXL_HOT_UNLOAD) == 0 && cmdList.size() == 3)
    {
        //unload
        if (LxlUnloadPlugin(cmdList[2]) == "")
            ERROR("Plugin no found!");
    }
    else if (cmd.find(LXL_HOT_RELOAD) == 0)
    {
        if (cmdList.size() == 2)
        {
            //reload all
            LxlReloadAllPlugins();
        }
        else if(cmdList.size() == 3)
        {
            //reload one
            if(!LxlReloadPlugin(cmdList[2]))
                ERROR("Plugin no found!");
        }
        else
            ERROR("Bad Command!");
    }
    else
        ERROR("Bad Command!");
    return false;
}

void ProcessStopServer(const string& cmd)
{
    if (cmd == "stop")
    {
        isServerStarted = false;
    }
}

bool CallPlayerCmdCallback(Player* player, const string& cmd)
{
    bool passToOriginalCmdEvent = true;

    for (auto engine : lxlModules)
    {
        EngineScope enter(engine);
        auto funcs = &(ENGINE_OWN_DATA()->playerCmdCallbacks);
        if (!funcs->empty())
            for (auto iter = funcs->begin(); iter != funcs->end(); ++iter)
            {
                string prefix = iter->first;
                if (cmd == prefix || (cmd.find(prefix) == 0 && cmd[prefix.size()] == ' '))
                    //如果命令与注册前缀全匹配，或者目标前缀后面为空格
                {
                    //Matched
                    Local<Array> args = Array::newArray();
                    if (cmd.size() > prefix.size())
                    {
                        //除了注册前缀之外还有额外参数
                        auto paras = SplitCmdParas(cmd.substr(prefix.size() + 1));
                        for (string para : paras)
                            args.add(String::newString(para));
                    }

                    auto res = iter->second.get().call({}, PlayerClass::newPlayer(player), args);
                    if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
                        passToOriginalCmdEvent = false;
                    break;
                }
            }
    }
    return passToOriginalCmdEvent;
}

bool CallServerCmdCallback(const string& cmd)
{
    bool passToOriginalCmdEvent = true;

    for (auto engine : lxlModules)
    {
        EngineScope enter(engine);
        auto funcs = &(ENGINE_OWN_DATA()->consoleCmdCallbacks);
        if (!funcs->empty())
            for (auto iter = funcs->begin(); iter != funcs->end(); ++iter)
            {
                string prefix = iter->first;
                if (cmd == prefix || (cmd.find(prefix) == 0 && cmd[prefix.size()] == ' '))
                    //如果命令与注册前缀全匹配，或者目标前缀后面为空格
                {
                    //Matched
                    Local<Array> args = Array::newArray();
                    if (cmd.size() > prefix.size())
                    {
                        //除了注册前缀之外还有额外参数
                        auto paras = SplitCmdParas(cmd.substr(prefix.size() + 1));
                        for (string para : paras)
                            args.add(String::newString(para));
                    }

                    auto res = iter->second.get().call({}, args);
                    if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
                        passToOriginalCmdEvent = false;
                    break;
                }
            }
    }
    return passToOriginalCmdEvent;
}

bool CallFormCallback(Player* player, unsigned formId, const string& data)
{
    bool passToBDS = true;

    FormCallbackKey key{ LXL_SCRIPT_LANG_TYPE,formId };
    try
    {
        auto callback = engineGlobalData->formCallbacks.at(key);

        EngineScope scope(callback.engine);
        auto res = callback.func.get().call({}, PlayerClass::newPlayer(player), JsonToValue(data));
        if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
            passToBDS = false;

        engineGlobalData->formCallbacks.erase(key);
    }
    catch (...)
    {
        ;
    }

    return passToBDS;
}