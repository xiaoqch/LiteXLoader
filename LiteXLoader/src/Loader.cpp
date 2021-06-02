#include <ScriptX/ScriptX.h>
#include <API/APIhelp.h>
#include <API/EngineOwnData.h>
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
using namespace script;

//基础库 & 依赖库
std::string baseLib;
std::vector<std::string> depends;

//前置声明
extern std::list<std::shared_ptr<ScriptEngine>> lxlModules;
void BindAPIs(std::shared_ptr<ScriptEngine> engine);

// 配置文件
extern INI_ROOT iniConf;

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

//预加载基础库
void LoadBaseLib()
{
    std::ifstream scriptBaseLib(LXL_SCRIPT_BASE_LIB_PATH);
    if(scriptBaseLib)
    {
        baseLib = string((std::istreambuf_iterator<char>(scriptBaseLib)),
            std::istreambuf_iterator<char>());
        scriptBaseLib.close();
        INFO("Script BaseLib Loaded.");
    }
}

//预加载依赖库
void LoadDepends()
{
    std::filesystem::directory_iterator deps(Raw_IniGetString(iniConf,"Main","DependsDir",LXL_SCRIPT_DEPENDS_DIR));
    for (auto& i : deps) {
        if (i.is_regular_file() && i.path().extension() == LXL_PLUGINS_SUFFIX)
        {
            try
            {
                depends.emplace_back(ReadFileFrom(i.path().string()));
                INFO("Dependence " + i.path().filename().string() + " Loaded.");
            }
            catch(std::exception e)
            {
                WARN("Fail to load dependence " + i.path().filename().string() + ".");
                WARN(e.what());
            }
            catch(...)
            {
                WARN("Fail to load dependence " + i.path().filename().string() + ".");
            }
        }
    }
}

//加载过程
void LoadScriptFile(const std::string& filePath)
{
    std::string scripts = ReadFileFrom(filePath);

    //启动引擎
    std::shared_ptr<ScriptEngine> engine = NewEngine();
    //setData
    std::static_pointer_cast<EngineOwnData>(engine->getData())->pluginName
        = std::filesystem::path(filePath).filename().u8string();
    
    lxlModules.push_back(engine);
    EngineScope enter(engine.get());

    //绑定API
    try{
        BindAPIs(engine);
    }
    catch(Exception& e)
    {
        ERROR("Fail in Binding APIs!\n");
        throw;
    }

    //加载基础库
    engine->eval(baseLib);

    //加载libs依赖库
    for (auto& i : depends) {
        engine->eval(i);
    }
    
    //加载脚本
    engine->eval(scripts);
}

//主加载
void LoadPlugins()
{
    INFO("Loading plugins...");
    std::filesystem::directory_iterator files(Raw_IniGetString(iniConf,"Main","PluginsDir",LXL_DEF_LOAD_DIR));
    for (auto& i : files) {
        if (i.is_regular_file() && i.path().extension() == LXL_PLUGINS_SUFFIX)
        {
            try
            {
                LoadScriptFile(i.path().string());
                INFO(i.path().filename().string() + " loaded.");
            }
            catch(Exception& e)
            {
                EngineScope enter(lxlModules.back().get());
                ERROR("Fail to load " + i.path().filename().string() + "!\n");
                ERRPRINT(e);
                ExitEngineScope exit;
                lxlModules.pop_back();
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
}