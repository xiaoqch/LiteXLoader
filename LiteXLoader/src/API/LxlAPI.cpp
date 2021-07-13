#include "APIhelp.h"
#include "LxlAPI.h"
#include "EngineGlobalData.h"
#include "EngineOwnData.h"
#include "../Loader.h"
#include <string>
#include <filesystem>
using namespace std;
using namespace script;

Local<Value> LxlGetVersion(const Arguments& args)
{
    try{
        Local<Object> ver = Object::newObject();
        ver.set("major", LXL_VERSION_MAJOR);
        ver.set("minor", LXL_VERSION_MINOR);
        ver.set("build", LXL_VERSION_BUILD);
        ver.set("isBeta", LXL_VERSION_IS_BETA);
        return ver;
    }
    CATCH("Fail in LxlGetVersion!")
}

Local<Value> LxlListPlugins(const Arguments& args)
{
    try
    {
        Local<Array> plugins = Array::newArray();
        for(auto pluginName : engineGlobalData->pluginsList)
        {
            plugins.add(String::newString(pluginName));
        }
        return plugins;
    }
    CATCH("Fail in LxlListPlugins!")
}