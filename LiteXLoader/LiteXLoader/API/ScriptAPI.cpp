#include "ScriptAPI.h"
#include "APIHelp.h"
#include <Kernel/System.h>
#include "EngineOwnData.h"
#include <windows.h>
#include <chrono>
#include <map>
#include <thread>
#include <memory>
using namespace std;

//////////////////// APIs ////////////////////

Local<Value> Log(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)

    try {
        for (int i = 0; i < args.size(); ++i)
            PrintValue(std::cout, args[i]);
        std::cout << std::endl;
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in Log!")
}

Local<Value> GetTimeStr(const Arguments& args)
{
    try {
        return String::newString(Raw_GetDateTimeStr());
    }
    CATCH("Fail in GetTimeStr!")
}

Local<Value> GetTimeObj(const Arguments& args)
{
    try {
        SYSTEMTIME st;
        GetLocalTime(&st);
        Local<Object> res = Object::newObject();
        res.set("Y", Number::newNumber((int)st.wYear));
        res.set("M", Number::newNumber((int)st.wMonth));
        res.set("D", Number::newNumber((int)st.wDay));
        res.set("h", Number::newNumber((int)st.wHour));
        res.set("m", Number::newNumber((int)st.wMinute));
        res.set("s", Number::newNumber((int)st.wSecond));
        res.set("ms", Number::newNumber((int)st.wMilliseconds));
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
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    try {
        bool isFunc = args[0].getKind() == ValueKind::kFunction;
        if (!isFunc && args[0].getKind() != ValueKind::kString)
        {
            ERROR("Unknown type of time task!");
            return Local<Value>();
        }

        Global<Value> func{ args[0] };
        int timeout = args[1].toInt();
        if (timeout <= 0)
            timeout = 1;

        timeTaskMap[++timeTaskId] = true;
        std::thread task([engine{ EngineScope::currentEngine() }, isFunc{ std::move(isFunc) },
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
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    try {
        bool isFunc = args[0].getKind() == ValueKind::kFunction;
        if (!isFunc && args[0].getKind() != ValueKind::kString)
        {
            ERROR("Unknown type of time task!");
            return Local<Value>();
        }

        Global<Value> func{ args[0] };
        int timeout = args[1].toInt();
        if (timeout <= 0)
            timeout = 1;

        timeTaskMap[++timeTaskId] = true;
        std::thread task([engine{ EngineScope::currentEngine() }, isFunc{ std::move(isFunc) },
            func{ std::move(func) }, timeout{ std::move(timeout) }, id{ timeTaskId }]()
        {
            auto sleepTime = std::chrono::milliseconds(timeout);
            while (timeTaskMap[id])
            {
                std::this_thread::sleep_for(sleepTime);
                EngineScope enter(engine);
                if (isFunc)
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
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber)

    try {
        timeTaskMap.at(args[0].toInt()) = false;
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in ClearInterval!")
        catch (const std::out_of_range& e)
    {
        ERROR("Time task id no found!");
    }
    return Boolean::newBoolean(false);
}