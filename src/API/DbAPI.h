#pragma once
#include "ScriptX.h"
#include <string>
#include <memory>
#include "../Kernel/Db.h"
using namespace script;

//////////////////// Classes ////////////////////

class DbClass : public ScriptClass
{
private:
    DB_TYPE kvdb;

public:
	explicit DbClass(const string &dir);

    bool isValid()
    { return kvdb != nullptr; }

    Local<Value> get(const Arguments& args);
    Local<Value> set(const Arguments& args);
    Local<Value> del(const Arguments& args);
};

//////////////////// APIs ////////////////////

Local<Value> ConfInit(const Arguments& args);

Local<Value> ConfJsonSet(const Arguments& args);
Local<Value> ConfJsonGet(const Arguments& args);
Local<Value> ConfJsonDelete(const Arguments& args);

Local<Value> ConfIniSet(const Arguments& args);
Local<Value> ConfIniGetStr(const Arguments& args);
Local<Value> ConfIniGetInt(const Arguments& args);
Local<Value> ConfIniGetFloat(const Arguments& args);
Local<Value> ConfIniGetBool(const Arguments& args);
Local<Value> ConfIniDeleteSec(const Arguments& args);
Local<Value> ConfIniDeleteKey(const Arguments& args);

Local<Value> ConfReload(const Arguments& args);
Local<Value> ConfGetPath(const Arguments& args);
Local<Value> ConfRead(const Arguments& args);
Local<Value> ConfWrite(const Arguments& args);

Local<Value> OpenDB(const Arguments& args);