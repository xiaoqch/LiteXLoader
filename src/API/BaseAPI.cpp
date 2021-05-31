#include "BaseAPI.h"
#include "APIhelp.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include "EngineOwnData.h"
#include "../Kernel/Base.h"
#include "../Kernel/System.h"
#include <chrono>
#include <map>
#include <memory>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<IntPos> IntPosBuilder =
    defineClass<IntPos>("IntPos")
        .constructor(&IntPos::create)
        .instanceProperty("x", &IntPos::getX, &IntPos::setX)
        .instanceProperty("y", &IntPos::getY, &IntPos::setY)
        .instanceProperty("z", &IntPos::getZ, &IntPos::setZ)
        .instanceProperty("dim", &IntPos::getDim, &IntPos::setDim)
        .build();

ClassDefine<FloatPos> FloatPosBuilder =
    defineClass<FloatPos>("FloatPos")
        .constructor(&FloatPos::create)
        .instanceProperty("x", &FloatPos::getX, &FloatPos::setX)
        .instanceProperty("y", &FloatPos::getY, &FloatPos::setY)
        .instanceProperty("z", &FloatPos::getZ, &FloatPos::setZ)
        .instanceProperty("dim", &FloatPos::getDim, &FloatPos::setDim)
        .build();


//////////////////// IntPos 生成函数 ////////////////////   

IntPos* IntPos::create(const Arguments& args)
{
    if(args.size() < 3)
        return nullptr;
    try
    {
        IntPos *p = new IntPos(args.thiz());
        p->x = args[0].asNumber().toInt64();
        p->y = args[1].asNumber().toInt64();
        p->z = args[2].asNumber().toInt64();
        p->dim = args[3].asNumber().toInt32();
        return p;
    }
    catch(...)
    {
        return nullptr;
    }
}

Local<Object> IntPos::newPos(int x, int y, int z, int dim)
{
    return EngineScope::currentEngine()->newNativeClass<IntPos>(x,y,z,dim);
}

Local<Object> IntPos::newPos(const BlockPos &b, int dim)
{
    return IntPos::newPos(b.x, b.y, b.z, dim);
}

Local<Object> IntPos::newPos(const IntVec4 &v)
{
    return IntPos::newPos(v.x, v.y, v.z, v.dim);
}

IntPos* IntPos::extractPos(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<IntPos>(v))
        return EngineScope::currentEngine()->getNativeInstance<IntPos>(v);
    else
        return nullptr;
}

//////////////////// FloatPos 生成函数 ////////////////////

FloatPos* FloatPos::create(const Arguments& args)
{
    if(args.size() < 3)
        return nullptr;
    try
    {
        FloatPos *p = new FloatPos(args.thiz());
        p->x = args[0].asNumber().toFloat();
        p->y = args[1].asNumber().toFloat();
        p->z = args[2].asNumber().toFloat();
        p->dim = args[3].asNumber().toInt32();
        return p;
    }
    catch(...)
    {
        return nullptr;
    }
}

Local<Object> FloatPos::newPos(double x, double y, double z, int dim)
{
    return EngineScope::currentEngine()->newNativeClass<FloatPos>(x,y,z,dim);
}

Local<Object> FloatPos::newPos(const Vec3 &v, int dim)
{
    return FloatPos::newPos(v.x, v.y, v.z, dim);
}

Local<Object> FloatPos::newPos(const FloatVec4 &v)
{
    return FloatPos::newPos(v.x, v.y, v.z, v.dim);
}

FloatPos* FloatPos::extractPos(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<FloatPos>(v))
        return EngineScope::currentEngine()->getNativeInstance<FloatPos>(v);
    else
        return nullptr;
}

//////////////////// APIs ////////////////////

Local<Value> Runcmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        return Boolean::newBoolean(Raw_Runcmd(args[0].asString().toString()));
    }
    CATCH("Fail in RunCmd!")
}

Local<Value> RuncmdEx(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        std::pair<bool, string> result = Raw_RuncmdEx(args[0].asString().toString());
        Local<Object> resObj = Object::newObject();
        resObj.set("result",result.first);
        resObj.set("output",result.second);
        return resObj;
    }
    CATCH("Fail in RunCmdEx!")
}

Local<Value> RegisterPlayerCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kFunction)
        
    if(args.size() >= 4)
        CHECK_ARG_TYPE(args[3],ValueKind::kNumber)

    try{
        string cmd = args[0].asString().toString();
        int level = 0;
        (ENGINE_OWN_DATA()->playerCmdCallbacks)[cmd] = args[2].asFunction();

        if(args.size() >= 4)
        {
            int newLevel = args[3].asNumber().toInt32();
            if(newLevel >= 0 && newLevel <= 3)
                level = newLevel;
        }

        Raw_RegisterCmd(cmd,args[1].asString().toString(),level);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in RegisterPlayerCmd!")
}

Local<Value> RegisterConsoleCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kFunction)

    try{
        DEBUG("Enter RegisterConsoleCmd API");
        string cmd = args[0].asString().toString();
        (ENGINE_OWN_DATA()->consoleCmdCallbacks)[cmd] = args[2].asFunction();

        DEBUG("Before Raw_RegisterCmd");
        Raw_RegisterCmd(cmd,args[1].asString().toString(),4);
        DEBUG("After Raw_RegisterCmd");

        return Boolean::newBoolean(true);
    }
    CATCH("Fail in RegisterConsoleCmd!")
}

Local<Value> Log(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    try{
        for(int i = 0; i < args.size(); ++i)
            PrintValue(std::cout,args[i]);
        std::cout << std::endl;
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in Log!")
}

Local<Value> GetTimeStr(const Arguments& args)
{
    try{
        return String::newString(Raw_GetDateTimeStr());
    }
    CATCH("Fail in GetTimeStr!")
}

Local<Value> GetTimeObj(const Arguments& args)
{
    try{
        SYSTEMTIME st;
        GetLocalTime(&st);
        Local<Object> res = Object::newObject();
        res.set("Y",Number::newNumber((int)st.wYear));
        res.set("M",Number::newNumber((int)st.wMonth));
        res.set("D",Number::newNumber((int)st.wDay));
        res.set("h",Number::newNumber((int)st.wHour));
        res.set("m",Number::newNumber((int)st.wMinute));
        res.set("s",Number::newNumber((int)st.wSecond));
        res.set("ms",Number::newNumber((int)st.wMilliseconds));
        return res;
    }
    CATCH("Fail in GetTimeNow!")
}

Local<Value> RandomGuid(const Arguments& args)
{
    return String::newString(Raw_RandomGuid());
}

Local<Value> GetLxlVersion(const Arguments& args)
{
    return String::newString(LXL_VERSION);
}


// Time Task
static unsigned long long nowTimeTaskId = 0;
std::map<int32_t,Global<Function>> funcMap;
void inline RemoveFromMap(int32_t id)
{
    funcMap.erase(id);
}

#define TASK_ID msg.data0
#define CODE_STR msg.ptr0
#define CODE_FUNC msg.data2
#define INTERVAL_WAIT msg.data1

// setTimeout
void FuncTimeout(script::utils::Message& msg)
{
    using namespace script::utils;
    EngineScope enter((ScriptEngine*)(msg.tag));
    funcMap[CODE_FUNC].get().call();
    RemoveFromMap(CODE_FUNC);
}
void CodeTimeout(script::utils::Message& msg)
{
    using namespace script::utils;
    ScriptEngine* engine = (ScriptEngine*)(msg.tag);
    EngineScope enter(engine);
    string *toEval = (string*)CODE_STR;
    engine->eval(*toEval);
    if(CODE_STR != nullptr)
    {
        delete CODE_STR;
        CODE_STR = nullptr;
    }
}

Local<Value> SetTimeout(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)

    using namespace script::utils;
    try{
        bool isFunc = args[0].getKind()==ValueKind::kFunction;
        Message msg(isFunc ? FuncTimeout : CodeTimeout, nullptr);
        msg.tag = EngineScope::currentEngine();
        if(isFunc)
        {
            CODE_STR = nullptr;
            funcMap.insert_or_assign(nowTimeTaskId,args[0].asFunction());
            CODE_FUNC = nowTimeTaskId; 
        }
        else
            CODE_STR = new string(args[0].asString().toString());
        int wait = args[1].asNumber().toInt32();
        if(wait < 0)
            wait = 0;
        TASK_ID = nowTimeTaskId;

        extern std::shared_ptr<ScriptEngine> globalEngine;
        globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(wait));
        return Number::newNumber((int64_t)nowTimeTaskId++);
    }
    CATCH("Fail in SetTimeout!")
}

// setInterval
void FuncInterval(script::utils::Message& msg)
{
    using namespace script::utils;
    extern std::shared_ptr<ScriptEngine> globalEngine;
    EngineScope enter((ScriptEngine*)(msg.tag));
    funcMap[CODE_FUNC].get().call();
    globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(INTERVAL_WAIT));
}
void CodeInterval(script::utils::Message& msg)
{
    using namespace script::utils;
    ScriptEngine* engine = (ScriptEngine*)(msg.tag);
    EngineScope enter(engine);
    string *toEval = (string*)CODE_STR;
    engine->eval(*toEval);

    extern std::shared_ptr<ScriptEngine> globalEngine;
    globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(INTERVAL_WAIT));
}

Local<Value> SetInterval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)

    using namespace script::utils;
    try{
        bool isFunc = args[0].getKind()==ValueKind::kFunction;
        Message msg(isFunc ? FuncInterval : CodeInterval, nullptr);
        msg.tag = EngineScope::currentEngine();
        if(isFunc)
        {
            CODE_STR = nullptr;
            funcMap.insert_or_assign(nowTimeTaskId,args[0].asFunction());
            CODE_FUNC = nowTimeTaskId; 
        }
        else
            CODE_STR = new string(args[0].asString().toString());
        int wait = args[1].asNumber().toInt32();
        if(wait < 0)
            wait = 0;
        TASK_ID = nowTimeTaskId;
        INTERVAL_WAIT = wait;

        extern std::shared_ptr<ScriptEngine> globalEngine;
        globalEngine->messageQueue()->postMessage(msg,std::chrono::milliseconds(wait));
        return Number::newNumber((int64_t)nowTimeTaskId++);
    }
    CATCH("Fail in SetInterval!")
}

// ClearInterval
Local<Value> ClearInterval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)

    try{
        using namespace script::utils;

        extern std::shared_ptr<ScriptEngine> globalEngine;
        bool res = globalEngine->messageQueue()->removeMessageIf(
            [&args](Message& msg) -> MessageQueue::RemoveMessagePredReturnType
            {
                if(TASK_ID == args[0].asNumber().toInt64())   //IntervalID
                {
                    if(CODE_STR != nullptr)
                    {
                        delete CODE_STR;
                        CODE_STR = nullptr;
                    }
                    else
                        RemoveFromMap(CODE_FUNC);
                    return MessageQueue::RemoveMessagePredReturnType::kRemove;
                }
                else
                    return MessageQueue::RemoveMessagePredReturnType::kDontRemove;
            }
        );
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in ClearInterval!")
}