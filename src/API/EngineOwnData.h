#include "APIhelp.h"
#include "ScriptX.h"
#include <fstream>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <memory>
#include "../Nlohmann/fifo_map.hpp"
#include "../Nlohmann/json.hpp"
#include "../Kernel/Db.h"

struct EngineOwnData_MapCmp
{
    bool operator() (string const &a,string const &b) const
    {
        if(a.size()!=b.size())
            return a.size()>b.size();
        else
            return a>b;
    }
};
class Player;

enum GlobalConfType { json, ini };
class minIni;

#define CmdCallback_MapType std::map<std::string,script::Global<script::Function>,EngineOwnData_MapCmp>
struct EngineOwnData
{
    //BaseAPI
    CmdCallback_MapType playerCmdCallbacks;

    //ServerAPI
    CmdCallback_MapType consoleCmdCallbacks;

    //LoggerAPI
    bool toConsole = true;
    std::ofstream fout;
    Player *player = nullptr;
    string title = "";
    int logLevel = 0;

    //PlayerAPI
    std::unordered_map<std::string,script::Global<script::Value>> playerDataDB;

    //DB API
    string confPath;
    GlobalConfType confType = GlobalConfType::json;
    fifo_json jsonConf;
    INI_TYPE *iniConf;
};

// 引擎附加数据
#define ENGINE_OWN_DATA() (std::static_pointer_cast<EngineOwnData>(EngineScope::currentEngine()->getData()))