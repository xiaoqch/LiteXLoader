#include "APIHelp.h"
#include "NetworkAPI.h"
#include <Kernel/System.h>
#include <string>
#include <vector>
using namespace std;
using namespace script;

Local<Value> HttpGet(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    CHECK_ARG_TYPE(args[1], ValueKind::kFunction)

    try
    {
        Global<Function> callbackFunc{ args[1].asFunction() };

        return Boolean::newBoolean(Raw_HttpGet(args[0].toStr(),
            [callback{ std::move(callbackFunc) }, engine{ EngineScope::currentEngine() }]
            (int status, string body)
        {
            EngineScope scope(engine);
            try
            {
                callback.get().call({}, Number::newNumber(status), String::newString(body));
            }
            catch (const Exception& e)
            {
                ERROR("HttpGet Callback Failed!");
                ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
                ERRPRINT(e);
            }
        }));
    }
    CATCH("Fail in HttpGet")
}

Local<Value> HttpPost(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 4)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    CHECK_ARG_TYPE(args[1], ValueKind::kString)
    CHECK_ARG_TYPE(args[2], ValueKind::kString)
    CHECK_ARG_TYPE(args[3], ValueKind::kFunction)

    try
    {
        Global<Function> callbackFunc{ args[3].asFunction() };

        return Boolean::newBoolean(Raw_HttpPost(args[0].toStr(), args[1].toStr(), args[2].toStr(),
            [callback{ std::move(callbackFunc) }, engine{ EngineScope::currentEngine() }]
            (int status, string data)
        {
            EngineScope scope(engine);
            try
            {
                callback.get().call({}, Number::newNumber(status), String::newString(data));
            }
            catch (const Exception& e)
            {
                ERROR("HttpPost Callback Failed!");
                ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
                ERRPRINT(e);
            }
        }));
    }
    CATCH("Fail in HttpPost")
}