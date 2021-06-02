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
    auto root = new SimpleIni;
    root->SetUnicode(true);
    if (root->LoadFile(path.c_str()) >= SI_FAIL)
    {
        delete root;
        return nullptr;
    }
    else
    {
        root->filePath = path;
        return root;
    }
}

bool Raw_IniClose(INI_ROOT ini)
{
    delete ini;
    return true;
}

bool Raw_IniSetInt(INI_ROOT ini, const string & sec, const string & key, int value)
{
    bool isOk = ini->SetLongValue(sec.c_str(), key.c_str(), value) < SI_FAIL;
    ini->SaveFile(ini->filePath.c_str());
    return isOk;
}

bool Raw_IniSetFloat(INI_ROOT ini, const string & sec, const string & key, float value)
{
    bool isOk = ini->SetDoubleValue(sec.c_str(),key.c_str(),value) < SI_FAIL;
    ini->SaveFile(ini->filePath.c_str());
    return isOk;
}

bool Raw_IniSetString(INI_ROOT ini, const string & sec, const string & key, const string & value)
{
    bool isOk = ini->SetValue(sec.c_str(),key.c_str(),value.c_str()) < SI_FAIL;
    ini->SaveFile(ini->filePath.c_str());
    return isOk;
}

bool Raw_IniSetBool(INI_ROOT ini, const string & sec, const string & key, bool value)
{
    bool isOk = ini->SetBoolValue(sec.c_str(),key.c_str(),value) < SI_FAIL;
    ini->SaveFile(ini->filePath.c_str());
    return isOk;
}

int Raw_IniGetInt(INI_ROOT ini, const string & sec, const string & key, int def)
{
    return ini->GetLongValue(sec.c_str(),key.c_str(),def);
}

float Raw_IniGetFloat(INI_ROOT ini, const string & sec, const string & key, float def)
{
    return (float)ini->GetDoubleValue(sec.c_str(),key.c_str(),def);
}

string Raw_IniGetString(INI_ROOT ini, const string & sec, const string & key, const string & def)
{
    return ini->GetValue(sec.c_str(),key.c_str(),def.c_str());
}

bool Raw_IniGetBool(INI_ROOT ini, const string & sec, const string & key, bool def)
{
    return ini->GetBoolValue(sec.c_str(),key.c_str(),def);
}

bool Raw_IniDeleteKey(INI_ROOT ini, const std::string & sec, const std::string & key)
{
    return ini->Delete(sec.c_str(),key.c_str(),true);
}