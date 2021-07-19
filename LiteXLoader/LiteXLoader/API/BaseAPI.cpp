#include "BaseAPI.h"
#include "pch.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "BlockAPI.h"
#include "EntityAPI.h"
#include "EngineOwnData.h"
#include <Kernel/Base.h>
#include <Kernel/System.h>
#include <Kernel/pch.h>
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
        .instanceProperty("dim", &IntPos::getDim)
        .instanceProperty("dimid", &IntPos::getDimId, &IntPos::setDimId)
        .build();

ClassDefine<FloatPos> FloatPosBuilder =
    defineClass<FloatPos>("FloatPos")
        .constructor(&FloatPos::create)
        .instanceProperty("x", &FloatPos::getX, &FloatPos::setX)
        .instanceProperty("y", &FloatPos::getY, &FloatPos::setY)
        .instanceProperty("z", &FloatPos::getZ, &FloatPos::setZ)
        .instanceProperty("dim", &FloatPos::getDim)
        .instanceProperty("dimid", &FloatPos::getDimId, &FloatPos::setDimId)
        .build();


//////////////////// IntPos ////////////////////   

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

Local<Value> IntPos::getDim()
{
    string name;
    switch (dim)
    {
    case 0:
        name = _TRS("base.getDimName.0");
        break;
    case 1:
        name = _TRS("base.getDimName.1");
        break;
    case 2:
        name = _TRS("base.getDimName.2");
        break;
    default:
        name = _TRS("base.getDimName.unknown");
        break;
    }
    return String::newString(name);
}

//////////////////// FloatPos ////////////////////

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

Local<Value> FloatPos::getDim()
{
    string name;
    switch (dim)
    {
    case 0:
        name = _TRS("base.getDimName.0");
        break;
    case 1:
        name = _TRS("base.getDimName.1");
        break;
    case 2:
        name = _TRS("base.getDimName.2");
        break;
    default:
        name = _TRS("base.getDimName.unknown");
        break;
    }
    return String::newString(name);
}

//////////////////// APIs ////////////////////

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
int timeTaskId = 0;
std::unordered_map<int, bool> timeTaskMap;

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
        
        timeTaskMap[++timeTaskId] = true;
        std::thread task([engine{EngineScope::currentEngine()}, isFunc{std::move(isFunc)},
            func{ std::move(func) }, timeout{ std::move(timeout) }, id{ timeTaskId }]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

            if (timeTaskMap[id])
            {
                EngineScope enter(engine);
                if (isFunc)
                    func.get().asFunction().call();
                else
                    engine->eval(func.get().toStr());
            }
            timeTaskMap[id] = false;
        });
        task.detach();

        return Number::newNumber(timeTaskId);
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
        
        timeTaskMap[++timeTaskId] = true;
        std::thread task([engine{EngineScope::currentEngine()}, isFunc{std::move(isFunc)},
            func{std::move(func)}, timeout{std::move(timeout)}, id{ timeTaskId }]()
        {
            auto sleepTime = std::chrono::milliseconds(timeout);
            while(timeTaskMap[id])
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
        task.detach();

        return Number::newNumber(timeTaskId);
    }
    CATCH("Fail in SetInterval!")
}

// ClearInterval
Local<Value> ClearInterval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)

    try{
        timeTaskMap.at(args[0].toInt()) = false;
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in ClearInterval!")
    catch(const std::out_of_range &e)
    {
        ERROR("Time task id no found!");
    }
    return Boolean::newBoolean(false);
}