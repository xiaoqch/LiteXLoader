// Ignore error below
#include "ScriptX.h"
#include "LiteLoader/headers/lbpch.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <list>
#include <filesystem>
#include <fstream>
#include <exception>
#include "Configs.h"
#include "API/ConfigHelp.h"
#include "API/LogAPI.h"

using EnginePtr = std::shared_ptr<script::ScriptEngine>;
std::list<EnginePtr> modules;
void LoadScript(const std::string& path);
void BindAPIs(EnginePtr engine);
void RegisterDebug(EnginePtr engine);

void entry()
{
    INFO(std::string("LiteXLoader Script Plugin Loader for ") +
        LXL_SCRIPT_LANG_TYPE +" - Based on LiteLoader");
    INFO(std::string("Version ") + LXL_VERSION);
    std::filesystem::directory_iterator files
        (conf::getString("Main","PluginsDir",LXL_DEF_LOAD_PATH));

    INFO("Loading plugins...");
    for (auto& i : files) {
        if (i.is_regular_file() && i.path().extension() == LXL_PLUGINS_SUFFIX)
        {
            try
            {
                LoadScript(i.path().string());
                INFO(i.path().filename().string() + " loaded.");
            }
            catch(script::Exception& e)
            {
                script::EngineScope enter(modules.back().get());
                ERROR("Fail to load " + i.path().filename().string() + "!\n");
                PRINT(e);
                //modules.pop_back();
            }
            catch(std::exception& e)
            {
                ERROR("Fail to load " + i.path().filename().string() + "!");
                ERROR(e.what());
            }
            catch(...)
            {
                ERROR("Fail to load " + i.path().filename().string() + "!");
            }
        }
    }
    //Event::
}


/////////////////////////////////////////////

EnginePtr NewEngine()
{
    #if !defined(SCRIPTX_BACKEND_WEBASSEMBLY)
        return EnginePtr{
            new script::ScriptEngineImpl(),
                script::ScriptEngine::Deleter()
        };
    #else
        return EnginePtr{
            script::ScriptEngineImpl::instance(),
                [](void*) {}
        };
    #endif
}

void LoadScript(const std::string& filePath)
{
    std::ifstream scriptFile(filePath);
    if(!scriptFile)
    {
        throw std::exception("Fail to open file!");
        return;
    }
    std::string scripts((std::istreambuf_iterator<char>(scriptFile)),
        std::istreambuf_iterator<char>());
    scriptFile.close();

    //启动引擎
    EnginePtr engine = NewEngine();
    modules.push_back(engine);
    script::EngineScope enter(engine.get());

    //绑定API
    try{
        BindAPIs(engine);
    }
    catch(script::Exception& e)
    {
        ERROR("Fail in Binding APIs!\n");
        throw;
    }

    //加载基础库
    std::ifstream scriptBaseLib(LXL_SCRIPT_BASE_LIB_PATH);
    if(scriptBaseLib)
    {
        std::string baseLibs((std::istreambuf_iterator<char>(scriptBaseLib)),
            std::istreambuf_iterator<char>());
        scriptBaseLib.close();
        engine->eval(baseLibs);
    }
    //加载脚本
    engine->eval(scripts);
    
    //注册后台调试
    RegisterDebug(engine);
}

void RegisterDebug(EnginePtr engine)
{
    ;
}