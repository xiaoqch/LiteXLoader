#include "APIHelp.h"
#include "LxlAPI.h"
#include <Engine/GlobalShareData.h>
#include <Engine/EngineOwnData.h>
#include <Engine/LoaderHelper.h>
#include <Kernel/System.h>
#include <Engine/LoaderHelper.h>
#include <Configs.h>
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
        ver.set("revision", LXL_VERSION_REVISION);
        ver.set("isBeta", LXL_VERSION_IS_BETA);
        return ver;
    }
    CATCH("Fail in LxlGetVersion!")
}

Local<Value> LxlCheckVersion(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kNumber);

    try {
        if (args[0].toInt() > LXL_VERSION_MAJOR
            || (args.size() >= 2 && args[1].toInt() > LXL_VERSION_MINOR)
            || (args.size() >= 3 && args[2].toInt() > LXL_VERSION_REVISION))
        {
            return Boolean::newBoolean(false);
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in LxlCheckVersion!")
}

Local<Value> LxlListPlugins(const Arguments& args)
{
    try
    {
        Local<Array> plugins = Array::newArray();
        auto list = LxlListGlocalAllPlugins();
        for(auto pluginName : list)
        {
            plugins.add(String::newString(pluginName));
        }
        return plugins;
    }
    CATCH("Fail in LxlListPlugins!")
}

Local<Value> LxlRequire(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kString)

    try
    {
        bool existing = false;
        string require = args[0].toStr();
        string thisName = ENGINE_OWN_DATA()->pluginName;

        //已加载插件
        for (auto pluginName : globalShareData->pluginsList)
        {
            if (pluginName == require)
            {
                existing = true;
                break;
            }
        }
        if (existing)
        {
            INFO(thisName + _TRS("lxlapi.require.success") + require);
            return Boolean::newBoolean(true);
        }

        //插件目录
        existing = false;
        auto list = Raw_GetFilesList(LXL_PLUGINS_LOAD_DIR);
        for (auto fileName : list)
        {
            if (fileName == require)
            {
                existing = true;
                break;
            }
        }
        if (existing)
        { 
            bool success = LxlLoadPlugin(string(LXL_PLUGINS_LOAD_DIR) + "/" + require);
            if (success)
            {
                INFO(thisName + _TRS("lxlapi.require.success") + require);
                return Boolean::newBoolean(true);
            }
            else
            {
                ERROR(thisName + _TRS("lxlapi.require.fail"));
                return Boolean::newBoolean(false);
            }
        }

        //依赖库目录
        existing = false;
        list = Raw_GetFilesList(LXL_DEPENDS_DIR);
        for (auto fileName : list)
        {
            if (fileName == require)
            {
                existing = true;
                break;
            }
        }
        if (existing)
        {
            bool success = LxlLoadPlugin(string(LXL_DEPENDS_DIR) + "/" + require);
            if (success)
            {
                INFO(thisName + _TRS("lxlapi.require.success") + require);
                return Boolean::newBoolean(true);
            }
            else
            {
                ERROR(thisName + _TRS("lxlapi.require.fail"));
                return Boolean::newBoolean(false);
            }
        }

        //HTTP(s)下载
        if (args.size() == 1)
        {
            ERROR(thisName + _TRS("lxlapi.require.fail"));
            return Boolean::newBoolean(false);
        }

        string remotePath = args[1].toStr();
        int status;
        string result, downloadPath = string(LXL_DEPENDS_DIR) + "/" + require;

        if (!Raw_HttpGetSync(remotePath, &status, &result) || status != 200)
        {
            ERROR(thisName + _TRS("lxlapi.require.network.fail") + to_string(status));
            return Boolean::newBoolean(false);
        }
        Raw_FileWriteTo(downloadPath, result);

        INFO(thisName + _TRS("lxlapi.require.download.success") + downloadPath);

        //下载完毕安装
        bool success = LxlLoadPlugin(downloadPath);
        if (success)
        {
            INFO(thisName + _TRS("lxlapi.require.success") + require);
            return Boolean::newBoolean(true);
        }
        else
        {
            ERROR(thisName + _TRS("lxlapi.require.fail"));
            return Boolean::newBoolean(false);
        }
    }
    CATCH("Fail in LxlRequire!")
}

Local<Value> LxlEval(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    try
    {
        return EngineScope::currentEngine()->eval(args[0].toStr());
    }
    CATCH("Fail in LxlEval!")
}