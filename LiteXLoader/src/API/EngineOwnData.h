#include "APIhelp.h"
#include <Kernel/ThirdParty.h>
#include <Kernel/Db.h>

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

enum GlobalConfType { json, ini };
class Player;

struct EngineOwnData
{
    //BaseInfo
    std::string pluginName = "";
    std::string pluginPath = "";

    //BaseAPI
    std::map<std::string, Global<Function> ,EngineOwnData_MapCmp> playerCmdCallbacks;

    //ServerAPI
    std::map<std::string, Global<Function> ,EngineOwnData_MapCmp> consoleCmdCallbacks;

    //LoggerAPI
    bool toConsole = true;
    std::ofstream fout;
    Player *player = nullptr;
    std::string title = "";
    int maxLogLevel = 4;
    int consoleLogLevel = 4;
    int fileLogLevel = 4;
    int playerLogLevel = 4;

    //PlayerAPI
    std::unordered_map<std::string,Global<Value>> playerDataDB;
};

// 引擎附加数据
#define ENGINE_GET_DATA(e) (std::static_pointer_cast<EngineOwnData>((e)->getData()))
#define ENGINE_OWN_DATA() (std::static_pointer_cast<EngineOwnData>(EngineScope::currentEngine()->getData()))