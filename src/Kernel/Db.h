#include "Global.h"
#include <string>
#include <vector>
#include <memory>

#define DB_TYPE std::unique_ptr<KVDBImpl>

DB_TYPE Raw_NewDB(const std::string &dir);
bool Raw_DBGet(DB_TYPE &db, const std::string & key, std::string &val);
bool Raw_DBSet(DB_TYPE &db, const std::string & key, const std::string & val);
bool Raw_DBDel(DB_TYPE &db, const std::string & key);
std::vector<std::string> Raw_DBListKey(DB_TYPE db);
