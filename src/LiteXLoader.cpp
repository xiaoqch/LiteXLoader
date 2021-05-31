// Ignore error below
#include "ScriptX.h"
#include "API/APIhelp.h"
#include "API/EngineOwnData.h"
#include "Kernel/Db.h"
#include "Kernel/System.h"
#include <windows.h>
#include <string>
#include <exception>
#include <thread>
#include <chrono>
#include <memory>
#include <filesystem>
#include "Configs.h"
using namespace script;

//主引擎表
std::list<std::shared_ptr<ScriptEngine>> lxlModules;
//消息分发引擎
std::shared_ptr<ScriptEngine> globalEngine;
//调试引擎
std::shared_ptr<ScriptEngine> debugEngine;
bool globalDebug = false;
// 配置文件
INI_ROOT iniConf;
// 日志等级
int lxlLogLevel = 1;

void LoadBaseLib();
void LoadDepends();
void LoadPlugins();
void BindAPIs(std::shared_ptr<ScriptEngine> engine);
void InitGlobalData();

void entry()
{
    INFO(std::string("====== LiteXLoader Script Plugin Loader for ") + LXL_SCRIPT_LANG_TYPE +" ======");
    INFO(std::string("Version ") + LXL_VERSION);

    Raw_DirCreate(std::filesystem::path(LXL_CONFIG_PATH).remove_filename().u8string());
    iniConf = Raw_IniOpen(LXL_CONFIG_PATH);
    lxlLogLevel = Raw_IniGetInt(iniConf,"Main","LxlLogLevel",1);
    LoadBaseLib();
    LoadDepends();
    
    LoadPlugins();
    InitGlobalData();
    Raw_IniClose(iniConf);
}

/////////////////////////////////////////////

void InitGlobalData()
{
    // 主消息循环
    globalEngine = NewEngine();
    std::thread([]() {
        EngineScope enter(globalEngine.get());
        globalEngine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopAndWait);
    }).detach();

    // GC循环
    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(Raw_IniGetInt(iniConf,"Advanced","GCInterval",20)));
        for(auto engine : lxlModules)
        {
            EngineScope enter(engine.get());
            engine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
        }
        globalEngine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
        debugEngine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
    }).detach();

    //后台调试
    debugEngine = NewEngine();
    //setData
    std::static_pointer_cast<EngineOwnData>(debugEngine->getData())->pluginName = "__DEBUGING_ENGINE__";
    
    EngineScope enter(debugEngine.get());
    try{
        BindAPIs(debugEngine);
    }
    catch(Exception& e)
    {
        ERROR("Fail in starting Debug Engine!\n");
        throw;
    }
}