#include "Db.h"
#include <filesystem>
#include "ThirdParty.h"
using namespace std;

DB_ROOT Raw_NewDB(const string &dir)
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

bool Raw_DBGet(DB_ROOT &db, const string & key, string &val)
{
    return db->get(key,val);
}

bool Raw_DBSet(DB_ROOT &db, const string & key, const string & val)
{
    db->put(key,val);
    return true;
}

bool Raw_DBDel(DB_ROOT &db, const string & key)
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
vector<string> Raw_DBListKey(DB_ROOT db)
{
    keyList.clear();
    db->iter(DBListKey_Helper);
    return keyList;
}

INI_ROOT Raw_IniOpen(const string &path)
{
    return new minIni(path);
}

bool Raw_IniClose(INI_ROOT ini)
{
    delete ini;
    return true;
}

bool Raw_IniSetInt(INI_ROOT ini, const string & sec, const string & key, int value)
{
    return ini->put(sec,key,value);
}

bool Raw_IniSetFloat(INI_ROOT ini, const string & sec, const string & key, float value)
{
    return ini->put(sec,key,value);
}

bool Raw_IniSetString(INI_ROOT ini, const string & sec, const string & key, const string & value)
{
    return ini->put(sec,key,value);
}

bool Raw_IniSetBool(INI_ROOT ini, const string & sec, const string & key, bool value)
{
    return ini->put(sec,key,value);
}

int Raw_IniGetInt(INI_ROOT ini, const string & sec, const string & key, int default)
{
    return ini->geti(sec,key,default);
}

float Raw_IniGetFloat(INI_ROOT ini, const string & sec, const string & key, float default)
{
    return ini->getf(sec,key,default);
}

string Raw_IniGetString(INI_ROOT ini, const string & sec, const string & key, const string & default)
{
    return ini->gets(sec,key,default);
}

bool Raw_IniGetBool(INI_ROOT ini, const string & sec, const string & key, bool default)
{
    return ini->getbool(sec,key,default);
}

bool Raw_IniDeleteKey(INI_ROOT ini, const std::string & sec, const std::string & key)
{
    return ini->del(sec,key);
}

bool Raw_IniDeleteSec(INI_ROOT ini, const std::string & sec)
{
    return ini->del(sec);
}