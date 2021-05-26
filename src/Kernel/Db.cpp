#include "Db.h"
#include <filesystem>
using namespace std;

DB_TYPE Raw_NewDB(const string &dir)
{
    if(!filesystem::is_directory(dir))
        return nullptr;
    else
    {
        try
        {
            return MakeKVDB(dir);
        }
        catch(...)
        {
            return nullptr;
        }
    }
}

bool Raw_DBGet(DB_TYPE &db, const string & key, string &val)
{
    return db->get(key,val);
}

bool Raw_DBSet(DB_TYPE &db, const string & key, const string & val)
{
    db->put(key,val);
    return true;
}

bool Raw_DBDel(DB_TYPE &db, const string & key)
{
    db->del(key);
    return true;
}

vector<string> keyList;
bool DBListKey_Helper(const string_view &key)
{
    keyList.push_back(string(key));
    return true;
}
std::vector<std::string> Raw_DBListKey(DB_TYPE db)
{
    keyList.clear();
    db->iter(DBListKey_Helper);
    return keyList;
}
