#include "BaseAPI.h"
#include "APIhelp.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include "EngineOwnData.h"
#include <Kernel/Base.h>
#include <Kernel/System.h>
#include <windows.h>
#include <chrono>
#include <map>
#include <memory>
#include <thread>
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
        p->x = args[0].asNumber().toInt32();
        p->y = args[1].asNumber().toInt32();
        p->z = args[2].asNumber().toInt32();
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
    CHECK_ARGS_COUNT(args,3)
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
    CHECK_ARGS_COUNT(args,3)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kFunction)

    try{
        string cmd = args[0].asString().toString();
        (ENGINE_OWN_DATA()->consoleCmdCallbacks)[cmd] = args[2].asFunction();

        Raw_RegisterCmd(cmd,args[1].asString().toString(),4);

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


/////////////// Helper ///////////////
std::map<unsigned int, DWORD> timeTaskMap;

Local<Value> SetTimeout(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)

    try{
        bool isFunc = args[0].getKind() == ValueKind::kFunction;
        if(!isFunc && args[0].getKind() != ValueKind::kString)
        {
            ERROR("Unknown type of time task!");
            return Local<Value>();
        }

        Global<Value> func{args[0]};
        int timeout = args[1].toInt();
        if(timeout <= 0)
            timeout = 1;
        
        std::thread task([engine{EngineScope::currentEngine()}, isFunc{std::move(isFunc)},
            func{std::move(func)}, timeout{std::move(timeout)}]()
        {
            timeTaskMap[Raw_GetSystemThreadIdFromStdThread(std::this_thread::get_id())]
                    = GetCurrentThreadId();
            
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
            EngineScope enter(engine);
            if(isFunc)
                func.get().asFunction().call();
            else
                engine->eval(func.get().toStr());
        });

        auto id = Raw_GetSystemThreadIdFromStdThread(task.get_id());
        task.detach();
        return Number::newNumber((long long)id);
    }
    CATCH("Fail in SetTimeout!")
}

Local<Value> SetInterval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)

    try{
        bool isFunc = args[0].getKind() == ValueKind::kFunction;
        if(!isFunc && args[0].getKind() != ValueKind::kString)
        {
            ERROR("Unknown type of time task!");
            return Local<Value>();
        }

        Global<Value> func{args[0]};
        int timeout = args[1].toInt();
        if(timeout <= 0)
            timeout = 1;
        
        std::thread task([engine{EngineScope::currentEngine()}, isFunc{std::move(isFunc)},
            func{std::move(func)}, timeout{std::move(timeout)}]()
        {
            timeTaskMap[Raw_GetSystemThreadIdFromStdThread(std::this_thread::get_id())]
                    = GetCurrentThreadId();
            auto sleepTime = std::chrono::milliseconds(timeout);
            while(true)
            {
                std::this_thread::sleep_for(sleepTime);
                EngineScope enter(engine);
                if(isFunc)
                    func.get().asFunction().call();
                else
                    engine->eval(func.get().toStr());
                ExitEngineScope exit;
            }
        });

        auto id = Raw_GetSystemThreadIdFromStdThread(task.get_id());
        task.detach();
        return Number::newNumber((long long)id);
    }
    CATCH("Fail in SetInterval!")
}

// ClearInterval
Local<Value> ClearInterval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)

    try{
        DWORD id=timeTaskMap.at((unsigned int)args[0].asNumber().toInt64());     //########### 前面返回的ID一直是0 ###########
        return Boolean::newBoolean(Raw_KillThread(id));
    }
    CATCH("Fail in ClearInterval!")
    catch(const std::out_of_range &e)
    {
        ERROR("Time task id no found!");
    }
    return Boolean::newBoolean(false);
}