#include "SystemAPI.h"
#include "APIHelp.h"
#include <Kernel/System.h>
#include <filesystem>
#include <fstream>
using namespace script;
using namespace std::filesystem;

Local<Value> SystemCmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kFunction);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kNumber);

    try {
        Global<Function> callbackFunc{ args[1].asFunction() };

        return Boolean::newBoolean(Raw_NewProcess("cmd /c" + args[0].toStr(),
            [callback{ std::move(callbackFunc) }, engine{ EngineScope::currentEngine() }]
        (int exitCode, string output)
        {
            EngineScope scope(engine);
            try
            {
                callback.get().call({}, Number::newNumber(exitCode), String::newString(output));
            }
            catch (const Exception& e)
            {
                ERROR("SystemCmd Callback Failed!");
                ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
                ERRPRINT(e);
            }
        }
        , args.size() >= 3 ? args[2].toInt() : -1));
    }
    CATCH("Fail in SystemCmd");
}

Local<Value> SystemNewProcess(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kFunction);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kNumber);

    try {
        Global<Function> callbackFunc{ args[1].asFunction() };

        return Boolean::newBoolean(Raw_NewProcess(args[0].toStr(),
            [callback{ std::move(callbackFunc) }, engine{ EngineScope::currentEngine() }]
        (int exitCode, string output)
        {
            EngineScope scope(engine);
            try
            {
                callback.get().call({}, Number::newNumber(exitCode), String::newString(output));
            }
            catch (const Exception& e)
            {
                ERROR("SystemNewProcess Callback Failed!");
                ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
                ERRPRINT(e);
            }
        }
        , args.size() >= 3 ? args[2].toInt() : -1));
    }
    CATCH("Fail in SystemCmd");
}