#include "APIHelp.h"
#include "DataAPI.h"
#include <fstream>
#include <string>
#include <sstream>
#include <exception>
#include <Engine/EngineOwnData.h>
#include <Kernel/System.h>
#include <Kernel/Data.h>
#include <Kernel/Utils.h>

using namespace script;
using namespace std;


//////////////////// Class Definition ////////////////////

ClassDefine<DbClass> DbClassBuilder =
    defineClass<DbClass>("LXL_Db")
        .constructor(nullptr)
        .instanceFunction("get", &DbClass::get)
        .instanceFunction("set", &DbClass::set)
        .instanceFunction("delete", &DbClass::del)
        .instanceFunction("close", &DbClass::close)
        .instanceFunction("listKey", &DbClass::listKey)
        .build();

ClassDefine<ConfJsonClass> ConfJsonClassBuilder =
    defineClass<ConfJsonClass>("LXL_ConfJson")
        .constructor(nullptr)
        .instanceFunction("get", &ConfJsonClass::get)
        .instanceFunction("set", &ConfJsonClass::set)
        .instanceFunction("delete", &ConfJsonClass::del)
        .instanceFunction("reload", selectOverloadedFunc<Local<Value> (ConfJsonClass::*)(const Arguments&)>(&ConfJsonClass::reload))
        .instanceFunction("close", selectOverloadedFunc<Local<Value>(ConfJsonClass::*)(const Arguments&)>(&ConfJsonClass::close))
        .instanceFunction("getPath", &ConfJsonClass::getPath)
        .instanceFunction("read", &ConfJsonClass::read)
        .instanceFunction("write", &ConfJsonClass::write)
        .build();

ClassDefine<ConfIniClass> ConfIniClassBuilder =
    defineClass<ConfIniClass>("LXL_ConfIni")
        .constructor(nullptr)
        .instanceFunction("set", &ConfIniClass::set)
        .instanceFunction("getStr", &ConfIniClass::getStr)
        .instanceFunction("getInt", &ConfIniClass::getInt)
        .instanceFunction("getFloat", &ConfIniClass::getFloat)
        .instanceFunction("getBool", &ConfIniClass::getBool)
        .instanceFunction("delete", &ConfIniClass::del)
        .instanceFunction("reload", selectOverloadedFunc<Local<Value>(ConfIniClass::*)(const Arguments&)>(&ConfIniClass::reload))
        .instanceFunction("close", selectOverloadedFunc<Local<Value>(ConfIniClass::*)(const Arguments&)>(&ConfIniClass::close))
        .instanceFunction("getPath", &ConfIniClass::getPath)
        .instanceFunction("read", &ConfIniClass::read)
        .instanceFunction("write", &ConfIniClass::write)
        .build();


//////////////////// Classes Db ////////////////////

//生成函数
Local<Value> DbClass::newDb(const string& dir)
{
    auto newp = new DbClass(dir);

    if (newp->isValid())
        return newp->getScriptObject();
    else
    {
        delete newp;
        return Local<Value>();
    }
}

DbClass::DbClass(const string &dir)
    :ScriptClass(ScriptClass::ConstructFromCpp<DbClass>{})
{
    kvdb = Raw_NewDB(dir);
}

DbClass::~DbClass()
{
    if (isValid())
    {
        Raw_DBClose(kvdb);
        kvdb = nullptr;
    }
}

Local<Value> DbClass::get(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        if (!isValid())
            return Local<Value>();

        string res;
        if(!Raw_DBGet(kvdb,args[0].asString().toString(),res))
            return Local<Value>();
        else
            return JsonToValue(res);
    }
    CATCH("Fail in DbGet!")
}

Local<Value> DbClass::set(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        if (!isValid())
            return Local<Value>();

        return Boolean::newBoolean(Raw_DBSet(kvdb,args[0].asString().toString(),ValueToJson(args[1])));
    }
    CATCH("Fail in DbSet!")
}

Local<Value> DbClass::del(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        if (!isValid())
            return Local<Value>();

        return Boolean::newBoolean(Raw_DBDel(kvdb,args[0].asString().toString()));
    }
    CATCH("Fail in DbDel!")
}

Local<Value> DbClass::close(const Arguments& args)
{
    try
    {
        if (isValid())
        {
            Raw_DBClose(kvdb);
            kvdb = nullptr;
        }
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in DbClose!")
}

Local<Value> DbClass::listKey(const Arguments& args)
{
    try
    {
        if (!isValid())
            return Local<Value>();

        auto list = Raw_DBListKey(kvdb);
        Local<Array> arr = Array::newArray();
        for (auto& key : list)
        {
            arr.add(String::newString(key));
        }
        return arr;
    }
    CATCH("Fail in DbListKey!")
}


//////////////////// Classes ConfBase ////////////////////

ConfBaseClass::ConfBaseClass(const string& dir)
    :confPath(dir)
{ }

Local<Value> ConfBaseClass::getPath(const Arguments& args)
{
    try
    {
        return String::newString(confPath);
    }
    CATCH("Fail in confGetPath!")
}

Local<Value> ConfBaseClass::read(const Arguments& args)
{
    try
    {
        string content;
        if (!Raw_FileReadFrom(confPath, content))
            return Local<Value>();
        else
            return String::newString(content);
    }
    CATCH("Fail in confRead!")
}

//////////////////// Classes ConfJson ////////////////////

//生成函数
Local<Value> ConfJsonClass::newConf(const string& path, const string& defContent)
{
    auto newp = new ConfJsonClass(path,defContent);
    return newp->getScriptObject();
}

ConfJsonClass::ConfJsonClass(const string& path, const string& defContent)
    :ScriptClass(ScriptClass::ConstructFromCpp<ConfJsonClass>{}), ConfBaseClass(path)
{
    jsonConf = Raw_JsonOpen(path, defContent);
}

ConfJsonClass::~ConfJsonClass()
{
    close();
}

Local<Value> ConfJsonClass::get(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        return JsonToValue(jsonConf.at(args[0].toStr()));
    }
    catch (const std::out_of_range& e)
    {
        return Local<Value>();
    }
    catch (exception& e)
    {
        return args.size() >= 2 ? args[1] : Local<Value>();
    }
    CATCH("Fail in confJsonGet!")
}

Local<Value> ConfJsonClass::set(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        jsonConf[args[0].toStr()] = JSON_VALUE::parse(ValueToJson(args[1]));
        return Boolean::newBoolean(flush());
    }
    catch (exception& e)
    {
        return args.size() >= 2 ? args[1] : Local<Value>();
    }
    CATCH("Fail in confJsonSet!");
}

Local<Value> ConfJsonClass::del(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        if (jsonConf.erase(args[0].toStr()) <= 0)
            return Boolean::newBoolean(false);

        return Boolean::newBoolean(flush());
    }
    catch (exception& e)
    {
        return args.size() >= 2 ? args[1] : Local<Value>();
    }
    CATCH("Fail in confJsonDelete!");
}

Local<Value> ConfJsonClass::reload(const Arguments& args)
{
    try
    {
        return Boolean::newBoolean(reload());
    }
    catch (exception& e)
    {
        ERROR("Fail to parse json content in file!");
        ERRPRINT(e.what());
        return Boolean::newBoolean(false);
    }
    CATCH("Fail in confJsonReload!");
}

Local<Value> ConfJsonClass::close(const Arguments& args)
{
    try
    {
        return Boolean::newBoolean(close());
    }
    CATCH("Fail in confJsonClose!");
}

Local<Value> ConfJsonClass::write(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        bool res = Raw_FileWriteTo(confPath, args[0].toStr());
        reload();
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in confJsonWrite!");
}

bool ConfJsonClass::flush()
{
    ofstream jsonFile(confPath);
    if (jsonFile.is_open())
    {
        jsonFile << jsonConf.dump(4);
        jsonFile.close();
        return true;
    }
    else
        return false;
}

bool ConfJsonClass::close()
{
    reload();
    return true;
}

bool ConfJsonClass::reload()
{
    string jsonTexts;
    if (!Raw_FileReadFrom(confPath, jsonTexts))
        return false;

    jsonConf = JSON_VALUE::parse(jsonTexts);
    return true;
}


//////////////////// Classes ConfIni ////////////////////

//生成函数
Local<Value> ConfIniClass::newConf(const string& path, const string& defContent)
{
    auto newp = new ConfIniClass(path,defContent);
    if (newp)
        return newp->getScriptObject();
    else
        return Local<Value>();
}

ConfIniClass::ConfIniClass(const string& path, const string& defContent)
    :ScriptClass(ScriptClass::ConstructFromCpp<ConfIniClass>{}), ConfBaseClass(path)
{
    iniConf = Raw_IniOpen(path, defContent);
}

ConfIniClass::~ConfIniClass()
{
    close();
}

bool ConfIniClass::flush()
{
    return iniConf->SaveFile(iniConf->filePath.c_str(), true);
}

bool ConfIniClass::close()
{
    if (isValid())
    {
        reload();
        Raw_IniClose(iniConf);
        iniConf = nullptr;
    }
    return true;
}
bool ConfIniClass::reload()
{
    if (!isValid())
        return false;

    Raw_IniClose(iniConf);
    iniConf = Raw_IniOpen(confPath);
    return true;
}

Local<Value> ConfIniClass::set(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 3);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);

    try
    {
        if (!isValid())
            return Local<Value>();

        string section = args[0].toStr();
        string key = args[1].toStr();
        switch (args[2].getKind())
        {
        case ValueKind::kString:
            Raw_IniSetString(iniConf, section, key, args[2].toStr());
            break;
        case ValueKind::kNumber:
            if (CheckIsFloat(args[2]))
                Raw_IniSetFloat(iniConf, section, key, (float)args[2].asNumber().toDouble());
            else
                Raw_IniSetInt(iniConf, section, key, args[2].toInt());
            break;
        case ValueKind::kBoolean:
            Raw_IniSetBool(iniConf, section, key, args[2].asBoolean().value());
            break;
        default:
            ERROR("Ini file don't support this type of data!");
            return Local<Value>();
            break;
        }
        flush();
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in confIniSet!")
}

Local<Value> ConfIniClass::getStr(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kString)

    try
    {
        if (!isValid())
            return Local<Value>();

        return String::newString(Raw_IniGetString(iniConf, args[0].toStr(), args[1].toStr(), 
            args.size() >= 3 ? args[2].toStr() : ""));
    }
    CATCH("Fail in confIniGetStr!")
}

Local<Value> ConfIniClass::getInt(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kNumber)

    try
    {
        if (!isValid())
            return Local<Value>();

        return Number::newNumber(Raw_IniGetInt(iniConf, args[0].toStr(), args[1].toStr(), 
            args.size() >= 3 ? args[2].asNumber().toInt32() : 0));
    }
    CATCH("Fail in ConfIniGetInt!");
}

Local<Value> ConfIniClass::getFloat(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kNumber);

    try
    {
        if (!isValid())
            return Local<Value>();

        return Number::newNumber(Raw_IniGetFloat(iniConf, args[0].toStr(), args[1].toStr(), 
            args.size() >= 3 ? (float)args[2].asNumber().toDouble() : 0.0));
    }
    CATCH("Fail in ConfIniGetFloat!");
}

Local<Value> ConfIniClass::getBool(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kBoolean);

    try
    {
        if (!isValid())
            return Local<Value>();

        return Boolean::newBoolean(Raw_IniGetBool(iniConf, args[0].toStr(), args[1].toStr(), 
            args.size() >= 3 ? args[2].asBoolean().value() : false));
    }
    CATCH("Fail in ConfIniGetBool");
}

Local<Value> ConfIniClass::del(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);

    try
    {
        if (!isValid())
            return Local<Value>();

        bool res = Raw_IniDeleteKey(iniConf, args[0].toStr(), args[1].toStr());
        flush();
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in confIniDelete!");
}

Local<Value> ConfIniClass::reload(const Arguments& args)
{
    try
    {
        return Boolean::newBoolean(reload());
    }
    CATCH("Fail in confReload!")
}

Local<Value> ConfIniClass::write(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        bool res = Raw_FileWriteTo(confPath, args[0].toStr());
        reload();
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in confIniWrite!");
}

Local<Value> ConfIniClass::close(const Arguments& args)
{
    try
    {
        return Boolean::newBoolean(close());
    }
    CATCH("Fail in confClose!")
}


//////////////////// APIs ////////////////////

Local<Value> OpenConfig(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kString);
    if (args.size() >= 3)
        CHECK_ARG_TYPE(args[2], ValueKind::kString);

    try{
        string path = args[0].toStr();
        GlobalConfType confType = GlobalConfType::ini;

        if(path.empty())
            return Boolean::newBoolean(false);  

        if(args.size() >= 2)
        {
            string fileType = args[1].toStr();
            if(fileType == "json" || fileType == "Json")
                confType = GlobalConfType::json;
        }
        
        if(confType == GlobalConfType::ini)
        {
            if (args.size() >= 3)
                return ConfIniClass::newConf(path, args[2].toStr());
            else
                return ConfIniClass::newConf(path);
        }
        else    //json
        {
            if (args.size() >= 3)
                return ConfJsonClass::newConf(path, args[2].toStr());
            else
                return ConfJsonClass::newConf(path, "{}");
        }
    }
    CATCH("Fail in OpenConfig!");
}

Local<Value> OpenDB(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    return DbClass::newDb(args[0].toStr());
}

Local<Value> MoneySet(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        return Boolean::newBoolean(Raw_SetMoney(stoull(args[0].toStr()), args[1].asNumber().toInt64()));
    }
    CATCH("Fail in MoneySet!")
}

Local<Value> MoneyGet(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        return Number::newNumber(Raw_GetMoney(stoull(args[0].toStr())));
    }
    CATCH("Fail in MoneyGet!")
}

Local<Value> MoneyAdd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        return Boolean::newBoolean(Raw_AddMoney(stoull(args[0].toStr()), args[1].asNumber().toInt64()));
    }
    CATCH("Fail in MoneyAdd!")
}

Local<Value> MoneyReduce(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        return Boolean::newBoolean(Raw_ReduceMoney(stoull(args[0].toStr()), args[1].asNumber().toInt64()));
    }
    CATCH("Fail in MoneyReduce!")
}

Local<Value> MoneyTrans(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 3);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);
    CHECK_ARG_TYPE(args[2], ValueKind::kNumber);

    try
    {
        string note = "";
        if (args.size() >= 4 && args[3].getKind() == ValueKind::kString)
            note = args[3].toStr();
        return Boolean::newBoolean(Raw_TransMoney(stoull(args[0].toStr()), stoull(args[1].toStr()),
                args[2].asNumber().toInt64(), note));
    }
    CATCH("Fail in MoneyTrans!")
}

Local<Value> MoneyGetHintory(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        string res{ Raw_GetMoneyHist(stoull(args[0].toStr()), args[1].asNumber().toInt64()) };
        auto list = SplitStrWithPattern(res, "\n");
        // from -> to money time (note)

        Local<Array> arr = Array::newArray();

        string from, to, time1, time2, note, tmp;
        long long money;
        for(auto &str : list)
        {
            if (str.back() == '\n')
                str.pop_back();

            istringstream sin(str);
            Local<Object> obj = Object::newObject();

            note.clear();
            sin >> from >> tmp >> to >> money >> time1 >> time2;
            sin.get();
            getline(sin, note);
            if (note.front() == '(')
                note.erase(0, 1);
            if (note.back() == '\n')
                note.pop_back();
            if (note.back() == ')')
                note.pop_back();

            time1 += " " + time2;

            obj.set("from", String::newString(from));
            obj.set("to", String::newString(to));
            obj.set("money", Number::newNumber(money));
            obj.set("time", String::newString(time1));
            obj.set("note", String::newString(note));
            arr.add(obj);
        }
        return arr;
    }
    CATCH("Fail in MoneyGetHintory!")
}

Local<Value> MoneyClearHistory(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        return Boolean::newBoolean(Raw_ClearMoneyHist(args[0].asNumber().toInt64()));
    }
    CATCH("Fail in MoneyClearHistory!")
}

Local<Value> Xuid2Name(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        return String::newString(Raw_Xuid2Name(args[0].toStr()));
    }
    CATCH("Fail in Xuid2Name!")
}

Local<Value> Name2Xuid(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        return String::newString(Raw_Name2Xuid(args[0].toStr()));
    }
    CATCH("Fail in Name2Xuid!")
}

Local<Value> ToJson(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    if (args.size() >= 2)
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        int spaces = -1;
        if (args.size() >= 2)
        {
            int newSpaces = args[1].toInt();
            if (newSpaces > 0)
                spaces = newSpaces;
        }
        try
        {
            return String::newString(ValueToJson(args[0],spaces));
        }
        catch (...)
        {
            ERROR("Failed to transform into Json.");
            return Local<Value>();
        }
    }
    CATCH("Fail in ToJson!")
}

Local<Value> ParseJson(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        try
        {
            return JsonToValue(args[0].toStr());
        }
        catch (...)
        {
            ERROR("Failed to parse from Json.");
            return Local<Value>();
        }
    }
    CATCH("Fail in ParseJson!")
}

Local<Value> ToMD5(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        return String::newString(Raw_toMD5(args[0].toStr()));
    }
    CATCH("Fail in ToMD5!")
}

Local<Value> ToSHA1(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        return String::newString(Raw_toSHA1(args[0].toStr()));
    }
    CATCH("Fail in SHA1!")
}