#include "ScriptX.h"
#include <fstream>
#include <string>
#include <unordered_map>

class Player;
struct EngineOwnData
{
    //LoggerAPI
    bool toConsole = true;
    std::ofstream fout;
    Player *player = nullptr;
    string title = "";
    int logLevel = 0;

    //PlayerAPI
    std::unordered_map<std::string,script::Global<Value>> playerDataDB;
};