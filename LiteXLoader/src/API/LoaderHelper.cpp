#include <ScriptX/ScriptX.h>
#include <API/APIhelp.h>
#include <API/EngineGlobalData.h>
#include <API/EngineOwnData.h>
#include <API/EventAPI.h>
#include <Kernel/Db.h>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <exception>
#include <filesystem>
#include <memory>
#include <Configs.h>
#include <Loader.h>
#include "LoaderHelper.h"
#include "RemoteCall.h"
using namespace script;
using namespace std;

//读取辅助函数
std::string ReadFileFrom(const std::string &filePath)
{
    std::ifstream fRead(filePath);
    if(!fRead)
    {
        throw std::exception("Fail to open file!");
        return string();
    }
    std::string data((std::istreambuf_iterator<char>(fRead)),
        std::istreambuf_iterator<char>());
    fRead.close();
    return data;
}

//创建新引擎
ScriptEngine* NewEngine()
{
    ScriptEngine* engine;

#if !defined(SCRIPTX_BACKEND_WEBASSEMBLY)
    engine = new ScriptEngineImpl();
#else
    engine = ScriptEngineImpl::instance();
#endif

    engine->setData(make_shared<EngineOwnData>());
    return engine;
}

//加载插件
bool LxlLoadPlugin(const std::string& filePath)
{
    try
    {
        std::string scripts = ReadFileFrom(filePath);

        //启动引擎
        ScriptEngine* engine = NewEngine();
        lxlModules.push_back(engine);
        EngineScope enter(engine);

        //setData
        string pluginName = std::filesystem::path(filePath).filename().u8string();
        ENGINE_OWN_DATA()->pluginName = pluginName;
        ENGINE_OWN_DATA()->pluginPath = filePath;

        //绑定API
        try {
            BindAPIs(engine);
        }
        catch (const Exception& e)
        {
            ERROR("Fail in Binding APIs!\n");
            throw;
        }

        //加载libs依赖库
        try
        {
            for (auto& i : depends) {
                engine->eval(i);
            }
        }
        catch (const Exception& e)
        {
            ERROR("Fail in Loading Dependence Lib!\n");
            throw;
        }

        //加载脚本
        try
        {
            engine->eval(scripts);
        }
        catch (const Exception& e)
        {
            ERROR("Fail in Loading Script Plugin!\n");
            throw;
        }

        AddToGlobalPluginsList(pluginName);
        INFO(pluginName + " loaded.");
        return true;
    }
    catch (const Exception& e)
    {
        ScriptEngine* deleteEngine = lxlModules.back();
        EngineScope enter(deleteEngine);

        deleteEngine->getData().reset();
        ERROR("Fail to load " + filePath + "!\n");
        ERRPRINT(e);
        ExitEngineScope exit;

        lxlModules.pop_back();

        //############# Js delete v8崩溃 #############
        //deleteEngine->destroy();
    }
    catch (const std::exception& e)
    {
        ERROR("Fail to load " + filePath + "!");
        ERROR(e.what());
    }
    catch (...)
    {
        ERROR("Fail to load " + filePath + "!");
    }
    return false;
}

//卸载插件
string LxlUnloadPlugin(const std::string& name)
{
    string unloadedPath = "";
    for (int i = 0; i < lxlModules.size(); ++i)
    {
        ScriptEngine* engine = lxlModules[i];
        if (ENGINE_GET_DATA(engine)->pluginName == name)
        {
            unloadedPath = ENGINE_GET_DATA(engine)->pluginPath;

            RemoveFromGlobalPluginsList(name);
            LxlRemoveAllEventListeners(engine);
            LxlRemoveAllExportedFuncs(engine);
            engine->getData().reset();
            lxlModules.erase(lxlModules.begin() + i);
            //############# Js delete v8崩溃 #############
            //engine->destory();

            INFO("Plugin " + name + " unloaded.")
            break;
        }
    }
    return unloadedPath;
}

//重载插件
bool LxlReloadPlugin(const std::string& name)
{
    string unloadedPath = LxlUnloadPlugin(name);
    if (unloadedPath.empty())
        return false;
    return LxlLoadPlugin(unloadedPath);
}

//重载全部插件
bool LxlReloadAllPlugin(const std::string& name)
{
    auto pluginsList = lxlModules;
    for (auto& engine : lxlModules)
        LxlReloadPlugin(ENGINE_GET_DATA(engine)->pluginName);
}

vector<string> LxlListAllPlugins()
{
    return engineGlobalData->pluginsList;
}