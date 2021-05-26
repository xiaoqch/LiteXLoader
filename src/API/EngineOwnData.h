#include "ScriptX.h"
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>

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
struct EngineOwnData
{
    //BaseAPI
    std::map<std::string,script::Global<Function>,EngineOwnData_MapCmp> playerCmdCallbacks;

    //ServerAPI
    std::map<std::string,script::Global<Function>,EngineOwnData_MapCmp> consoleCmdCallbacks;

    //LoggerAPI
    bool toConsole = true;
    std::ofstream fout;
    Player *player = nullptr;
    string title = "";
    int logLevel = 0;

    //PlayerAPI
    std::unordered_map<std::string,script::Global<Value>> playerDataDB;
};

#define CmdCallback_MapType std::map<std::string,script::Global<Function>,EngineOwnData_MapCmp>