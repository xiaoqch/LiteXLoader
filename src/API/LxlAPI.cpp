#include "APIhelp.h"
#include "LxlAPI.h"
#include "EngineGlobalData.h"
#include <string>
#include <filesystem>
using namespace std;
using namespace script;

Local<Value> LxlGetVersion(const Arguments& args)
{
    return String::newString(LXL_VERSION);
}

extern void LoadScriptFile(const std::string& filePath);
Local<Value> LxlLoadPlugin(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string newPlugin = args[0].toStr();
    if(!filesystem::path(newPlugin).has_filename())
    {
        ERROR("Fail to load " + newPlugin + "!\n");
        return Boolean::newBoolean(false);
    }

    try
    {
        LoadScriptFile(newPlugin);
        return Boolean::newBoolean(true);
    }
    catch(Exception& e)
    {
        EngineScope enter(lxlModules.back().get());
        ERROR("Fail to load " + newPlugin + "!\n");
        ERRPRINT(e);
        ExitEngineScope exit;
        lxlModules.pop_back();
    }
    catch(std::exception& e)
    {
        ERROR("Fail to load " + newPlugin + "!");
        ERROR(e.what());
    }
    catch(...)
    {
        ERROR("Fail to load " + newPlugin + "!");
    }
    return Boolean::newBoolean(false);
}