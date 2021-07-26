#pragma once
#include "Global.h"
#include "ThirdParty.h"
#include <string>
#include <vector>
#include <memory>

class KVDBImpl;
class minIni;
#define DB_ROOT KVDBImpl*

//LevelDB
DB_ROOT Raw_NewDB(const std::string &dir);
bool Raw_DBGet(DB_ROOT &db, const std::string & key, std::string &val);
bool Raw_DBSet(DB_ROOT &db, const std::string & key, const std::string & val);
bool Raw_DBDel(DB_ROOT &db, const std::string & key);
std::vector<std::string> Raw_DBListKey(DB_ROOT &db);
bool Raw_DBClose(DB_ROOT& db);

//Json
JSON_ROOT Raw_JsonOpen(const std::string& path, const std::string& defContent = "");

//Ini
INI_ROOT Raw_IniOpen(const std::string &path, const std::string& defContent = "");
bool Raw_IniClose(INI_ROOT ini);

bool Raw_IniSetInt(INI_ROOT ini, const std::string & sec, const std::string & key, int value);
bool Raw_IniSetFloat(INI_ROOT ini, const std::string & sec, const std::string & key, float value);
bool Raw_IniSetString(INI_ROOT ini, const std::string & sec, const std::string & key, const std::string & value);
bool Raw_IniSetBool(INI_ROOT ini, const std::string & sec, const std::string & key, bool value);

int Raw_IniGetInt(INI_ROOT ini, const std::string & sec, const std::string & key, int def = 0);
float Raw_IniGetFloat(INI_ROOT ini, const std::string & sec, const std::string & key, float def = 0.0);
std::string Raw_IniGetString(INI_ROOT ini, const std::string & sec, const std::string & key, const std::string & def = "");
bool Raw_IniGetBool(INI_ROOT ini, const std::string & sec, const std::string & key, bool def = false);

bool Raw_IniDeleteKey(INI_ROOT ini, const std::string & sec, const std::string & key);


//LLMoney
bool Raw_InitEcnonmicSystem();

typedef long long money_t;
typedef unsigned long long xuid_t;

money_t Raw_GetMoney(xuid_t player);
bool Raw_SetMoney(xuid_t player, money_t money);
bool Raw_AddMoney(xuid_t player, money_t money);
bool Raw_ReduceMoney(xuid_t player, money_t money);
bool Raw_TransMoney(xuid_t player1, xuid_t player2, money_t money, string const& notes);
std::string Raw_GetMoneyHist(xuid_t player, int time);
bool Raw_ClearMoneyHist(int time);

//XuidDB
string Raw_Xuid2Name(string xuid);
string Raw_Name2Xuid(string name);