#include "APIhelp.h"
#include "ScriptX.h"
#include <fstream>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <memory>
#include "../Kernel/ThirdParty.h"
#include "../Kernel/Db.h"
using namespace script;

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
    //BaseAPI
    std::map<std::string, std::pair<ScriptEngine*,Global<Function>> ,EngineOwnData_MapCmp> playerCmdCallbacks;

    //ServerAPI
    std::map<std::string, std::pair<ScriptEngine*,Global<Function>> ,EngineOwnData_MapCmp> consoleCmdCallbacks;

    //LoggerAPI
    bool toConsole = true;
    std::ofstream fout;
    Player *player = nullptr;
    std::string title = "";
    int logLevel = 0;

    //PlayerAPI
    std::unordered_map<std::string,Global<Value>> playerDataDB;

    //DB API
    std::string confPath;
    GlobalConfType confType = GlobalConfType::json;
    JSON_ROOT jsonConf;
    INI_ROOT iniConf;

    //GUI API
    std::unordered_map<int, std::pair<ScriptEngine*,Global<Function>> > formCallbacks;
};

// 引擎附加数据
#define ENGINE_OWN_DATA() (std::static_pointer_cast<EngineOwnData>(EngineScope::currentEngine()->getData()))