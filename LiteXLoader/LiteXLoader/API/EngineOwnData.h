#include <Kernel/ThirdParty.h>
#include <Kernel/Db.h>
#include <ScriptX/ScriptX.h>
#include <fstream>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <memory>

static struct EngineOwnData_MapCmp
{
    bool operator() (std::string const &a, std::string const &b) const
    {
        if(a.size()!=b.size())
            return a.size()>b.size();
        else
            return a>b;
    }
};

struct FormCallbackData
{
    script::ScriptEngine* engine;
    script::Global<script::Function> func;
};

enum GlobalConfType { json, ini };
class Player;

struct EngineOwnData
{
    //基础信息
    std::string pluginName = "";
    std::string pluginPath = "";

    //玩家命令回调
    std::map<std::string, Global<Function> ,EngineOwnData_MapCmp> playerCmdCallbacks;

    //控制台命令回调
    std::map<std::string, Global<Function> ,EngineOwnData_MapCmp> consoleCmdCallbacks;

    //表单回调
    std::map<unsigned, FormCallbackData> formCallbacks;

    //LoggerAPI
    bool toConsole = true;
    std::ofstream fout;
    Player *player = nullptr;
    std::string title = "";
    int maxLogLevel = 4;
    int consoleLogLevel = 4;
    int fileLogLevel = 4;
    int playerLogLevel = 4;

    //玩家绑定数据
    std::unordered_map<std::string,Global<Value>> playerDataDB;
};

// 引擎附加数据
#define ENGINE_GET_DATA(e) (std::static_pointer_cast<EngineOwnData>((e)->getData()))
#define ENGINE_OWN_DATA() (std::static_pointer_cast<EngineOwnData>(EngineScope::currentEngine()->getData()))