#include "APIHelp.h"
#include "LoggerAPI.h"
#include "PlayerAPI.h"
#include <Kernel/System.h>
#include <Kernel/Player.h>
#include <Engine/EngineOwnData.h>
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
    if(globalConf->toConsole && globalConf->consoleLogLevel >= level)
    {
        if(!preString.empty())
            cout << preString;
        LogDataHelper(&cout, args);
    }
    if(globalConf->fout && globalConf->fileLogLevel >= level)
    {
        if(!preString.empty())
            globalConf->fout << preString;
        LogDataHelper(&(globalConf->fout), args);
    }
    if(globalConf->player && globalConf->playerLogLevel >= level && Raw_IsPlayerValid(globalConf->player))
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
        LogToEachTarget(globalConf, "", args, -2147483647);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerLog!")
}

Local<Value> LoggerDebug(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    try {
        auto globalConf = ENGINE_OWN_DATA();
        if (LOGGER_CHECK_DEBUG(globalConf->maxLogLevel))
        {
            string preString{ globalConf->title };
            if (!preString.empty())
                preString = "[" + preString + "]";
            preString = preString + "[" + GetTimeStrHelper() + " Debug] ";

            LogToEachTarget(globalConf, preString, args, 5);
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerDebug!")
}

Local<Value> LoggerInfo(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try {
        auto globalConf = ENGINE_OWN_DATA();
        if (LOGGER_CHECK_INFO(globalConf->maxLogLevel))
        {
            string preString{ globalConf->title };
            if (!preString.empty())
                preString = "[" + preString + "]";
            preString = preString + "[" + GetTimeStrHelper() + " Info] ";

            LogToEachTarget(globalConf, preString, args, 4);
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerInfo!")
}

Local<Value> LoggerWarn(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try {
        auto globalConf = ENGINE_OWN_DATA();
        if (LOGGER_CHECK_WARN(globalConf->maxLogLevel))
        {
            string preString{ globalConf->title };
            if (!preString.empty())
                preString = "[" + preString + "]";
            preString = preString + "[" + GetTimeStrHelper() + " Warning] ";

            LogToEachTarget(globalConf, preString, args, 3);
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerWarn!")
}

Local<Value> LoggerError(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try {
        auto globalConf = ENGINE_OWN_DATA();
        if (LOGGER_CHECK_ERROR(globalConf->maxLogLevel))
        {
            string preString{ globalConf->title };
            if (!preString.empty())
                preString = "[" + preString + "]";
            preString = preString + "[" + GetTimeStrHelper() + " Error] ";

            LogToEachTarget(globalConf, preString, args, 2);
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerError!")
}

Local<Value> LoggerFatal(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try {
        auto globalConf = ENGINE_OWN_DATA();
        if (LOGGER_CHECK_FATAL(globalConf->maxLogLevel))
        {
            string preString{ globalConf->title };
            if (!preString.empty())
                preString = "[" + preString + "]";
            preString = preString + "[" + GetTimeStrHelper() + " FATAL] ";

            LogToEachTarget(globalConf, preString, args, 1);
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerFatal!")
}

Local<Value> LoggerSetTitle(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try {
        ENGINE_OWN_DATA()->title = args[0].asString().toString();
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerSetTitle!")
}

///////////////// Helper /////////////////
void UpdateMaxLogLevel()
{
    auto data = ENGINE_OWN_DATA();
    data->maxLogLevel = data->consoleLogLevel;
    if (data->maxLogLevel < data->fileLogLevel)
        data->maxLogLevel = data->fileLogLevel;
    if (data->maxLogLevel < data->playerLogLevel)
        data->maxLogLevel = data->playerLogLevel;
}
///////////////// Helper /////////////////

Local<Value> LoggerSetConsole(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kBoolean)
    if(args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    try {
        ENGINE_OWN_DATA()->toConsole = args[0].asBoolean().value();
        if (args.size() >= 2)
        {
            ENGINE_OWN_DATA()->consoleLogLevel = args[1].toInt();
            UpdateMaxLogLevel();
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerSetConsole!")
}

Local<Value> LoggerSetFile(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    try {
        string newFile = args[0].asString().toString();
        Raw_AutoCreateDirs(newFile);

        ofstream *fout = &(ENGINE_OWN_DATA()->fout);
        if(fout->is_open())
            fout->close();
        fout->clear();
        fout->open(newFile,ios::app);

        if (args.size() >= 2)
        {
            ENGINE_OWN_DATA()->fileLogLevel = args[1].toInt();
            UpdateMaxLogLevel();
        }
        return Boolean::newBoolean(ENGINE_OWN_DATA()->fout.is_open());
    }
    CATCH("Fail in LoggerSetFile!")
}

Local<Value> LoggerSetPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    try {
        Player *p = PlayerClass::extractPlayer(args[0]);
        ENGINE_OWN_DATA()->player = p;

        if (args.size() >= 2)
        {
            ENGINE_OWN_DATA()->playerLogLevel = args[1].toInt();
            UpdateMaxLogLevel();
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LoggerSetPlayer!")
}

Local<Value> SetLogLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber)

    try {
        auto conf = ENGINE_OWN_DATA();
        conf->maxLogLevel = conf->consoleLogLevel = conf->fileLogLevel = conf->playerLogLevel = args[0].toInt();
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in SetLogLevel!")
}
