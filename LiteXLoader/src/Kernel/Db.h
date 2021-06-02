#pragma once
#include "Global.h"
#include "ThirdParty.h"
#include <string>
#include <vector>
#include <memory>

class KVDBImpl;
class minIni;
#define DB_ROOT std::unique_ptr<KVDBImpl>

//LevelDB
DB_ROOT Raw_NewDB(const std::string &dir);
bool Raw_DBGet(DB_ROOT &db, const std::string & key, std::string &val);
bool Raw_DBSet(DB_ROOT &db, const std::string & key, const std::string & val);
bool Raw_DBDel(DB_ROOT &db, const std::string & key);
std::vector<std::string> Raw_DBListKey(DB_ROOT db);


//Ini
INI_ROOT Raw_IniOpen(const std::string &path);
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