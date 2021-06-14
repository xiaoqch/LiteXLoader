#include "APIhelp.h"
#include "LoggerAPI.h"
#include "PlayerAPI.h"
#include <Kernel/System.h>
#include <Kernel/Player.h>
#include "EngineOwnData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace script;

////////////////// Helper //////////////////
string inline GetTimeStrHelper()
{
    return Raw_GetDateTimeStr();
}

string& StrReplace(string& str, const string& to_replaced, const string& new_str)
{
    for (string::size_type pos(0); pos != string::npos; pos += new_str.length())
    {
        pos = str.find(to_replaced, pos);
        if (pos != string::npos)
            str.replace(pos, to_replaced.length(), new_str);
        else
            break;
    }
    return str;
}
////////////////// Helper //////////////////

void inline LogDataHelper(ostream *outStream, const Arguments& args)
{
    for(int i = 0; i < args.size(); ++i)
        PrintValue(*outStream,args[i]);
    (*outStream) << endl;
}

void LogToEachTarget(shared_ptr<EngineOwnData> globalConf,
    const string &preString, const Arguments& args, int level)
{
    if(globalConf->toConsole && globalConf->consoleLogLevel <= level)
    {
        if(!preString.empty())
            cout << preString;
        LogDataHelper(&cout, args);
    }
    if(globalConf->fout && globalConf->fileLogLevel <= level)
    {
        if(!preString.empty())
            globalConf->fout << preString;
        LogDataHelper(&(globalConf->fout), args);
    }
    if(globalConf->player && globalConf->playerLogLevel <= level && Raw_IsPlayerValid(globalConf->player))
    {
        ostringstream ostr;
        LogDataHelper(&ostr, args);

        string sendStr{ preString + ostr.str() };
        StrReplace(sendStr, "[", "<");
        StrReplace(sendStr, "]", ">");
        Raw_Tell(globalConf->player, sendStr);
    }
}

Local<Value> LoggerLog(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    try{
        auto globalConf = ENGINE_OWN_DATA();
        LogToEachTarget(globalConf, "", args, 2147483647);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerLog!")
}

Local<Value> LoggerDebug(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    auto globalConf = ENGINE_OWN_DATA();
    if (globalConf->minLogLevel <= 0)
    {
        string preString{ globalConf->title };
        if (!preString.empty())
            preString = "[" + preString + "]";
        preString = preString + "[" + GetTimeStrHelper() + " Debug] ";

        LogToEachTarget(globalConf, preString, args, 0);
    }
    return Boolean::newBoolean(true);
}

Local<Value> LoggerInfo(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if (globalConf->minLogLevel <= 1)
    {
        string preString{ globalConf->title };
        if (!preString.empty())
            preString = "[" + preString + "]";
        preString = preString + "[" + GetTimeStrHelper() + " Info] ";

        LogToEachTarget(globalConf, preString, args, 1);
    }
    return Boolean::newBoolean(true);
}

Local<Value> LoggerWarn(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if (globalConf->minLogLevel <= 2)
    {
        string preString{ globalConf->title };
        if (!preString.empty())
            preString = "[" + preString + "]";
        preString = preString + "[" + GetTimeStrHelper() + " Warning] ";

        LogToEachTarget(globalConf, preString, args, 2);
    }
    return Boolean::newBoolean(true);
}

Local<Value> LoggerError(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if (globalConf->minLogLevel <= 3)
    {
        string preString{ globalConf->title };
        if (!preString.empty())
            preString = "[" + preString + "]";
        preString = preString + "[" + GetTimeStrHelper() + " Error] ";

        LogToEachTarget(globalConf, preString, args, 3);
    }
    return Boolean::newBoolean(true);
}

Local<Value> LoggerFatal(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if (globalConf->minLogLevel <= 4)
    {
        string preString{ globalConf->title };
        if (!preString.empty())
            preString = "[" + preString + "]";
        preString = preString + "[" + GetTimeStrHelper() + " FATAL] ";

        LogToEachTarget(globalConf, preString, args, 4);
    }
    return Boolean::newBoolean(true);
}

Local<Value> LoggerSetTitle(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    ENGINE_OWN_DATA()->title = args[0].asString().toString();
    return Boolean::newBoolean(true);
}

///////////////// Helper /////////////////
void UpdateMinLogLevel()
{
    auto data = ENGINE_OWN_DATA();
    data->minLogLevel = data->consoleLogLevel;
    if (data->minLogLevel > data->fileLogLevel)
        data->minLogLevel = data->fileLogLevel;
    if (data->minLogLevel > data->playerLogLevel)
        data->minLogLevel = data->playerLogLevel;
}
///////////////// Helper /////////////////

Local<Value> LoggerSetConsole(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kBoolean)
    if(args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    ENGINE_OWN_DATA()->toConsole = args[0].asBoolean().value();
    if (args.size() >= 2)
    {
        ENGINE_OWN_DATA()->consoleLogLevel = args[1].toInt();
        UpdateMinLogLevel();
    }
    return Boolean::newBoolean(true);
}

Local<Value> LoggerSetFile(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    string newFile = args[0].asString().toString();
    ofstream *fout = &(ENGINE_OWN_DATA()->fout);
    if(fout->is_open())
        fout->close();
    fout->clear();
    fout->open(newFile,ios::app);

    if (args.size() >= 2)
    {
        ENGINE_OWN_DATA()->fileLogLevel = args[1].toInt();
        UpdateMinLogLevel();
    }
    return Boolean::newBoolean(ENGINE_OWN_DATA()->fout.is_open());
}

Local<Value> LoggerSetPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    Player *p = PlayerClass::extractPlayer(args[0]);
    ENGINE_OWN_DATA()->player = p;

    if (args.size() >= 2)
    {
        ENGINE_OWN_DATA()->playerLogLevel = args[1].toInt();
        UpdateMinLogLevel();
    }
    return Boolean::newBoolean(true);
}

Local<Value> SetLogLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber)

    auto conf = ENGINE_OWN_DATA();
    conf->minLogLevel = conf->consoleLogLevel = conf->fileLogLevel = conf->playerLogLevel = args[0].toInt();
    return Boolean::newBoolean(true);
}
