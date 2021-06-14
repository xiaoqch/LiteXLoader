#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;
#include <Kernel/Db.h>

#include <string>
#include <memory>

//////////////////// Classes ////////////////////

class DbClass : public ScriptClass
{
private:
    DB_ROOT kvdb;

public:
	explicit DbClass(const string &dir);

    bool isValid()
    { return kvdb != nullptr; }

    Local<Value> get(const Arguments& args);
    Local<Value> set(const Arguments& args);
    Local<Value> del(const Arguments& args);

    static Local<Value> newDb(const string& dir);
};


class ConfBaseClass
{
protected:
    std::string confPath;

public:
    explicit ConfBaseClass(const string& dir);

    virtual Local<Value> reload(const Arguments& args) = 0;
    Local<Value> getPath(const Arguments& args);
    Local<Value> read(const Arguments& args);
    Local<Value> write(const Arguments& args);
};


class ConfJsonClass : public ScriptClass, public ConfBaseClass
{
private:
    JSON_ROOT jsonConf;

public:
    explicit ConfJsonClass(const string& path, const string& defContent);

    Local<Value> get(const Arguments& args);
    Local<Value> set(const Arguments& args);
    Local<Value> del(const Arguments& args);
    virtual Local<Value> reload(const Arguments& args) override;

    static Local<Value> newConf(const string& path, const string& defContent = "");
};


class ConfIniClass : public ScriptClass, public ConfBaseClass
{
private:
    INI_ROOT iniConf;

public:
    explicit ConfIniClass(const string& path, const string& defContent);

    Local<Value> set(const Arguments& args);
    Local<Value> getStr(const Arguments& args);
    Local<Value> getInt(const Arguments& args);
    Local<Value> getFloat(const Arguments& args);
    Local<Value> getBool(const Arguments& args);
    Local<Value> del(const Arguments& args);
    virtual Local<Value> reload(const Arguments& args) override;

    static Local<Value> newConf(const string& path, const string& defContent = "");
};

//////////////////// APIs ////////////////////

Local<Value> OpenConfig(const Arguments& args);

Local<Value> OpenDB(const Arguments& args);

Local<Value> MoneySet(const Arguments& args);
Local<Value> MoneyGet(const Arguments& args);
Local<Value> MoneyAdd(const Arguments& args);
Local<Value> MoneyReduce(const Arguments& args);
Local<Value> MoneyTrans(const Arguments& args);
Local<Value> MoneyGetHintory(const Arguments& args);
Local<Value> MoneyClearHistory(const Arguments& args);

Local<Value> ToJson(const Arguments& args);
Local<Value> FromJson(const Arguments& args);