#pragma once
#include "Global.h"
#include <string>
#include <vector>
#include <memory>

class KVDBImpl;
class minIni;
#define DB_TYPE std::unique_ptr<KVDBImpl>
#define INI_TYPE minIni

DB_TYPE Raw_NewDB(const std::string &dir);
bool Raw_DBGet(DB_TYPE &db, const std::string & key, std::string &val);
bool Raw_DBSet(DB_TYPE &db, const std::string & key, const std::string & val);
bool Raw_DBDel(DB_TYPE &db, const std::string & key);
std::vector<std::string> Raw_DBListKey(DB_TYPE db);

INI_TYPE* Raw_IniOpen(const std::string &path);
bool Raw_IniClose(INI_TYPE *ini);

bool Raw_IniSetInt(INI_TYPE *ini, const std::string & sec, const std::string & key, int value);
bool Raw_IniSetFloat(INI_TYPE *ini, const std::string & sec, const std::string & key, float value);
bool Raw_IniSetString(INI_TYPE *ini, const std::string & sec, const std::string & key, const std::string & value);
bool Raw_IniSetBool(INI_TYPE *ini, const std::string & sec, const std::string & key, bool value);

int Raw_IniGetInt(INI_TYPE *ini, const std::string & sec, const std::string & key, int default = 0);
float Raw_IniGetFloat(INI_TYPE *ini, const std::string & sec, const std::string & key, float default = 0.0);
std::string Raw_IniGetString(INI_TYPE *ini, const std::string & sec, const std::string & key, const std::string & default = "");
bool Raw_IniGetBool(INI_TYPE *ini, const std::string & sec, const std::string & key, bool default = false);

bool Raw_IniDeleteKey(INI_TYPE *ini, const std::string & sec, const std::string & key);
bool Raw_IniDeleteSec(INI_TYPE *ini, const std::string & sec);