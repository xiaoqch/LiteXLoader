#include "APIHelp.h"
#include <ScriptX/ScriptX.h>
#include <Kernel/Server.h>
using namespace script;

Local<Value> SetMotd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        return Boolean::newBoolean(Raw_SetServerMotd(args[0].asString().toString()));
    }
    CATCH("Fail in SetServerMotd!")
}

Local<Value> SetOnlinePlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)

    try{
        return Boolean::newBoolean(Raw_SetOnlinePlayer(args[0].asNumber().toInt32(),args[1].asNumber().toInt32()));
    }
    CATCH("Fail in SetOnlinePlayer!")
}

Local<Value> CrashBDS(const Arguments& args)
{
    *(int*)(uintptr_t)0 = 2;
    return Boolean::newBoolean(true);
}