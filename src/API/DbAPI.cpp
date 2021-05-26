#include "APIhelp.h"
#include "DbAPI.h"
#include <fstream>
#include <string>
#include <exception>
#include "EngineOwnData.h"
#include "../Kernel/System.h"
#include "../Kernel/db.h"
using namespace script;
using namespace std;


//////////////////// Class Definition ////////////////////

ClassDefine<DbClass> DbClassBuilder =
    defineClass<DbClass>("Db")
        .constructor(nullptr)
        .instanceFunction("get", &DbClass::get)
        .instanceFunction("set", &DbClass::set)
        .instanceFunction("delete", &DbClass::del)
        .build();


//////////////////// Classes ////////////////////

DbClass::DbClass(const string &dir)
    :ScriptClass(ScriptClass::ConstructFromCpp<DbClass>{})
{
    kvdb = Raw_NewDB(dir);
}

Local<Value> DbClass::get(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
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
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        return Boolean::newBoolean(Raw_DBSet(kvdb,args[0].asString().toString(),ValueToJson(args[1])));
    }
    CATCH("Fail in DbSet!")
}

Local<Value> DbClass::del(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        return Boolean::newBoolean(Raw_DBDel(kvdb,args[0].asString().toString()));
    }
    CATCH("Fail in DbDel!")
}


//////////////////// APIs ////////////////////

Local<Value> ConfInit(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    if(args.size() >= 2)
        CHECK_ARG_TYPE(args[2],ValueKind::kString)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[3],ValueKind::kString)

    try{
        string path = args[0].asString().toString();
        string fileType = args[1].asString().toString();
        GlobalConfType confType = GlobalConfType::json;

        if(path.empty())
            return Boolean::newBoolean(false);
        path = "./plugins/" + path + "/config";
        

        if(args.size() >= 2)
        {
            if(fileType == "ini")
                ENGINE_OWN_DATA()->confType = confType = GlobalConfType::ini;
            else if(fileType != "json")
                return Boolean::newBoolean(false);
        }
        
        if(confType == GlobalConfType::json)
        {
            path+=".json";
            ENGINE_OWN_DATA()->confPath = path;

            fifo_json *jsonConf = &(ENGINE_OWN_DATA()->jsonConf);
            if(!Raw_PathExists(path))
            {
                //创建新的
                if(args.size() >= 3)
                {
                    try
                    {
                        *jsonConf = fifo_json::parse(args[2].asString().toString());
                    }
                    catch(exception &e)
                    {
                        ERROR("Fail to parse default json content!");
                        ERRPRINT(e.what());
                        *jsonConf = fifo_json::array();
                    }
                }
                else
                    *jsonConf = fifo_json::array();

                ofstream jsonFile(path);
                if(jsonFile.is_open() && args.size() >= 3)
                    jsonFile << jsonConf->dump(4);
                jsonFile.close();
            }
            else
            {
                //已存在
                string jsonTexts;
                if(!Raw_FileReadAll(path,jsonTexts))
                    return Boolean::newBoolean(false);
                try
                {
                    *jsonConf = fifo_json::parse(jsonTexts);
                }
                catch(exception &e)
                {
                    ERROR("Fail to parse json content in file!");
                    ERRPRINT(e.what());
                    *jsonConf = fifo_json::array();
                }
            }
        }
        else
        {
            path+=".ini";
            ENGINE_OWN_DATA()->confPath = path;

            if(!Raw_PathExists(path))
            {
                //创建新的
                ofstream iniFile(path);
                if(iniFile.is_open() && args.size() >= 3)
                    iniFile << args[2].asString().toString();
                iniFile.close();
            }

            //已存在
            ENGINE_OWN_DATA()->iniConf = new minIni(path);
        }

        return Boolean::newBoolean(true);
    }
    CATCH("Fail in ConfInit!")
}

Local<Value> ConfJsonGet(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::json)
        {
            try
            {
                return JsonToValue(ENGINE_OWN_DATA()->jsonConf.at(args[0].asString().toString()));
            }
            catch(exception &e)
            {
                return args.size() >= 2 ? args[1] : Local<Value>();
            }
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfJsonGet!")
}

Local<Value> ConfJsonSet(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::json)
        {
            try
            {
                ENGINE_OWN_DATA()->jsonConf[args[0].asString().toString()] = ValueToJson(args[1]);
                //写回文件
                ofstream jsonFile(ENGINE_OWN_DATA()->confPath);
                if(jsonFile.is_open())
                {
                    jsonFile << ENGINE_OWN_DATA()->jsonConf.dump(4);
                    jsonFile.close();
                    return Boolean::newBoolean(true);
                }
                else
                    return Boolean::newBoolean(false);
            }
            catch(exception &e)
            {
                return args.size() >= 2 ? args[1] : Local<Value>();
            }
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfJsonSet!")
}

Local<Value> ConfIniSet(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,3)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::ini)
        {
            switch(args[1].getKind())
            {
                case ValueKind::kString:
                    ENGINE_OWN_DATA()->iniConf->put(args[0].asString().toString(),args[1].asString().toString(),args[2].asString().toString());
                    break;
                case ValueKind::kNumber:
                    ENGINE_OWN_DATA()->iniConf->put(args[0].asString().toString(),args[1].asString().toString(),(float)args[2].asNumber().toDouble());
                    break;
                case ValueKind::kBoolean:
                    ENGINE_OWN_DATA()->iniConf->put(args[0].asString().toString(),args[1].asString().toString(),args[2].asBoolean().value());
                    break;
                default:
                    ERROR("Ini file don't support this type of data!");
                    return Boolean::newBoolean(false);
                    break;
            }
            return Boolean::newBoolean(true);
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfIniSet!")
}

Local<Value> ConfIniGetStr(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kString)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::ini)
        {
            return String::newString(ENGINE_OWN_DATA()->iniConf->gets(args[0].asString().toString(),
                args[1].asString().toString(), args.size() >= 3 ? args[2].asString().toString() : ""));
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfIniGetStr!")
}

Local<Value> ConfIniGetInt(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kNumber)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::ini)
        {
            return Number::newNumber(ENGINE_OWN_DATA()->iniConf->geti(args[0].asString().toString(),
                args[1].asString().toString(), args.size() >= 3 ? args[2].asNumber().toInt32() : 0));
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfIniGetInt!")
}

Local<Value> ConfIniGetFloat(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kNumber)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::ini)
        {
            return Number::newNumber(ENGINE_OWN_DATA()->iniConf->getf(args[0].asString().toString(),
                args[1].asString().toString(), args.size() >= 3 ? args[2].asNumber().toFloat() : 0.0));
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfIniGetFloat!")
}

Local<Value> ConfIniGetBool(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    if(args.size() >= 3)
        CHECK_ARG_TYPE(args[2],ValueKind::kBoolean)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::ini)
        {
            return Boolean::newBoolean(ENGINE_OWN_DATA()->iniConf->getbool(args[0].asString().toString(),
                args[1].asString().toString(), args.size() >= 3 ? args[2].asBoolean().value() : false));
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfIniGetBool")
}

Local<Value> ConfJsonDelete(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::json)
        {
            try
            {
                return Boolean::newBoolean(ENGINE_OWN_DATA()->jsonConf.erase(args[0].asString().toString()) > 0);
            }
            catch(exception &e)
            {
                return args.size() >= 2 ? args[1] : Local<Value>();
            }
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfJsonDelete!")
}

Local<Value> ConfIniDeleteSec(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::ini)
        {
            return Boolean::newBoolean(ENGINE_OWN_DATA()->iniConf->del(args[0].asString().toString()));
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfIniDeleteSec!")
}

Local<Value> ConfIniDeleteKey(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)

    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::ini)
        {
            return Boolean::newBoolean(ENGINE_OWN_DATA()->iniConf->del(args[0].asString().toString(),args[1].asString().toString()));
        }
        else
        {
            ERROR("Wrong type of config file!");
            return Local<Value>();
        }
    }
    CATCH("Fail in ConfIniDeleteKey!")
}

Local<Value> ConfReload(const Arguments& args)
{
    try{
        if(ENGINE_OWN_DATA()->confType == GlobalConfType::json)
        {
            string jsonTexts;
            if(!Raw_FileReadAll(ENGINE_OWN_DATA()->confPath,jsonTexts))
                return Boolean::newBoolean(false);
            try
            {
                ENGINE_OWN_DATA()->jsonConf = fifo_json::parse(jsonTexts);
                return Boolean::newBoolean(true);
            }
            catch(exception &e)
            {
                ERROR("Fail to parse json content in file!");
                ERRPRINT(e.what());
            }
            return Boolean::newBoolean(false);
        }
        else
        {
            delete ENGINE_OWN_DATA()->iniConf;
            ENGINE_OWN_DATA()->iniConf = new minIni(ENGINE_OWN_DATA()->confPath);
            return Boolean::newBoolean(true);
        }
    }
    CATCH("Fail in ConfReload!")
}

Local<Value> ConfGetPath(const Arguments& args)
{
    return String::newString(ENGINE_OWN_DATA()->confPath);
}

Local<Value> ConfRead(const Arguments& args)
{
    string content;
    if(!Raw_FileReadAll(ENGINE_OWN_DATA()->confPath,content))
        return Local<Value>();
    else
        return String::newString(content);
}

Local<Value> ConfWrite(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    return Boolean::newBoolean(Raw_FileWriteAll(ENGINE_OWN_DATA()->confPath,args[0].asString().toString()));
}

Local<Value> OpenDB(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    auto newp = new DbClass(args[0].asString().toString());
    if(newp->isValid())
        return newp->getScriptObject();
    else
    {
        delete newp;
        return Local<Value>();
    }
}