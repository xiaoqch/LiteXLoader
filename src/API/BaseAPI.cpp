#include "BaseAPI.h"
#include "APIhelp.h"
using namespace script;


//////////////////// APIs ////////////////////

script::Local<script::Value> RunCmd(const script::Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        bool res = liteloader::runcmd(args[0].asString().toString());
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in RunCmd!")
}

script::Local<script::Value> RunCmdAs(const script::Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        
        //return Boolean::newBoolean(res);
    }
    CATCH("Fail in RunCmdAs!")
}

script::Local<script::Value> SendText(const script::Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        
        //return Boolean::newBoolean(res);
    }
    CATCH("Fail in SendText!")
}