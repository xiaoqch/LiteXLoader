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
////////////////// Helper //////////////////

void inline LogDataHelper(ostream *outStream, const Arguments& args)
{
    for(int i = 0; i < args.size(); ++i)
        PrintValue(*outStream,args[i]);
    (*outStream) << endl;
}

void LogToEachTarget(shared_ptr<EngineOwnData> globalConf,
    const string &preString, const Arguments& args)
{
    if(globalConf->toConsole)
    {
        if(!preString.empty())
            cout << preString;
        LogDataHelper(&cout, args);
    }
    if(globalConf->fout)
    {
        if(!preString.empty())
            globalConf->fout << preString;
        LogDataHelper(&(globalConf->fout), args);
    }
    if(globalConf->player && Raw_IsPlayerValid(globalConf->player))
    {
        ostringstream ostr;
        LogDataHelper(&ostr, args);
        Raw_Tell(globalConf->player,preString + ostr.str());
    }
}

Local<Value> LoggerLog(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    try{
        auto globalConf = ENGINE_OWN_DATA();
        LogToEachTarget(globalConf,"",args);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerLog!")
}

Local<Value> LoggerDebug(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    auto globalConf = ENGINE_OWN_DATA();
    if(globalConf->logLevel > 0)
        return Boolean::newBoolean(true);

    string preString{globalConf->title};
    if(!preString.empty())
        preString = "[" + preString + "] ";
    preString = preString + "[" + GetTimeStrHelper() + " Debug] ";

    LogToEachTarget(globalConf,preString,args);
    return Boolean::newBoolean(true);
}

Local<Value> LoggerInfo(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if(globalConf->logLevel > 1)
        return Boolean::newBoolean(true);

    string preString{globalConf->title};
    if(!preString.empty())
        preString = "[" + preString + "] ";
    preString = preString + "[" + GetTimeStrHelper() + " Info] ";

    LogToEachTarget(globalConf,preString,args);
    return Boolean::newBoolean(true);
}

Local<Value> LoggerWarn(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if(globalConf->logLevel > 2)
        return Boolean::newBoolean(true);

    string preString{globalConf->title};
    if(!preString.empty())
        preString = "[" + preString + "] ";
    preString = preString + "[" + GetTimeStrHelper() + " Warning] ";

    LogToEachTarget(globalConf,preString,args);
    return Boolean::newBoolean(true);
}

Local<Value> LoggerError(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if(globalConf->logLevel > 3)
        return Boolean::newBoolean(true);

    string preString{globalConf->title};
    if(!preString.empty())
        preString = "[" + preString + "] ";
    preString = preString + "[" + GetTimeStrHelper() + " Error] ";

    LogToEachTarget(globalConf,preString,args);
    return Boolean::newBoolean(true);
}

Local<Value> LoggerFatal(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    auto globalConf = ENGINE_OWN_DATA();
    if(globalConf->logLevel > 4)
        return Boolean::newBoolean(true);

    string preString{globalConf->title};
    if(!preString.empty())
        preString = "[" + preString + "] ";
    preString = preString + "[" + GetTimeStrHelper() + " FATAL] ";

    LogToEachTarget(globalConf,preString,args);
    return Boolean::newBoolean(true);
}

Local<Value> LoggerSetTitle(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    ENGINE_OWN_DATA()->title = args[0].asString().toString();
    return Boolean::newBoolean(true);
}

Local<Value> LoggerSetConsole(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kBoolean)

    ENGINE_OWN_DATA()->toConsole = args[0].asBoolean().value();
    return Boolean::newBoolean(true);
}

Local<Value> LoggerSetFile(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string newFile = args[0].asString().toString();
    ofstream *fout = &(ENGINE_OWN_DATA()->fout);
    if(fout->is_open())
        fout->close();
    fout->clear();
    fout->open(newFile,ios::app);
    return Boolean::newBoolean(ENGINE_OWN_DATA()->fout.is_open());
}

Local<Value> LoggerSetPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    Player *p = PlayerClass::extractPlayer(args[0]);
    ENGINE_OWN_DATA()->player = p;
    return Boolean::newBoolean(true);
}

Local<Value> SetLogLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)

    ENGINE_OWN_DATA()->logLevel = args[0].asNumber().toInt32();
    return Boolean::newBoolean(true);
}
