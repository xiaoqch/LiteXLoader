// Ignore error below
#include <ScriptX/ScriptX.h>
#include <API/APIhelp.h>
#include <API/EngineGlobalData.h>
#include <API/EngineOwnData.h>
#include <Kernel/Db.h>
#include <Kernel/System.h>
#include <windows.h>
#include <string>
#include <exception>
#include <thread>
#include <chrono>
#include <memory>
#include <filesystem>
#include <Configs.h>
using namespace script;

//主引擎表
std::list<std::shared_ptr<ScriptEngine>> lxlModules;
// 配置文件
INI_ROOT iniConf;
// 日志等级
int lxlLogLevel = 1;

extern void LoadBaseLib();
extern void LoadDepends();
extern void LoadPlugins();
extern void BindAPIs(std::shared_ptr<ScriptEngine> engine);
extern void LoadDebugEngine();

void entry()
{
    INFO(std::string("====== LiteXLoader Script Plugin Loader for ") + LXL_SCRIPT_LANG_TYPE +" ======");
    INFO(std::string("Version ") + LXL_VERSION);

    Raw_DirCreate(std::filesystem::path(LXL_CONFIG_PATH).remove_filename().u8string());
    iniConf = Raw_IniOpen(LXL_CONFIG_PATH);
    if (!iniConf)
        ERROR("Failed in loading configs of LXL!");
    lxlLogLevel = Raw_IniGetInt(iniConf,"Main","LxlLogLevel",1);

    //初始化全局数据
    InitEngineGlobalData();

    //初始化经济系统
    Raw_InitEcnonmicSystem();

    //预加载库
    LoadBaseLib();
    LoadDepends();
    
    //加载插件
    LoadPlugins();

    //注册后台调试
    LoadDebugEngine();

    //GC循环
    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(Raw_IniGetInt(iniConf, "Advanced", "GCInterval", 20)));
        for (auto engine : lxlModules)
        {
            EngineScope enter(engine.get());
            engine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
        }
    }).detach();    //############## loadPlugin加锁 ################

    Raw_IniClose(iniConf);
}