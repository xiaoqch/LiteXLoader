#pragma once
#include "ScriptX.h"
#include <string>
#include "../Kernel/Global.h"
using namespace script;

//////////////////// Classes ////////////////////
class IntPos : public IntVec4, public ScriptClass
{
public:
	explicit IntPos(const Local<Object>& scriptObj) : ScriptClass(scriptObj) {}
    static IntPos* create(const Arguments& args)
    {
        if(args.size() < 3)
            return nullptr;
        try
        {
            IntPos *p = new IntPos(args.thiz());
            p->x = args[0].asNumber().toInt64();
            p->y = args[1].asNumber().toInt64();
            p->z = args[2].asNumber().toInt64();
            p->dim = args[3].asNumber().toInt32();
            return p;
        }
        catch(...)
        {
            return nullptr;
        }
	}

    Local<Value> getX() { return Number::newNumber(x); }
    Local<Value> getY() { return Number::newNumber(y); }
    Local<Value> getZ() { return Number::newNumber(z); }
    Local<Value> getDim() { return Number::newNumber(dim); }
    void setX(const Local<Value>& value) { x = value.asNumber().toInt64(); }
    void setY(const Local<Value>& value) { y = value.asNumber().toInt64(); }
    void setZ(const Local<Value>& value) { z = value.asNumber().toInt64(); }
    void setDim(const Local<Value>& value) { dim = value.asNumber().toInt32(); }
};

class FloatPos : public FloatVec4, public ScriptClass
{
public:
	explicit FloatPos(const Local<Object>& scriptObj) : ScriptClass(scriptObj) {}
    static FloatPos* create(const Arguments& args)
    {
        if(args.size() < 3)
            return nullptr;
        try
        {
            FloatPos *p = new FloatPos(args.thiz());
            p->x = args[0].asNumber().toFloat();
            p->y = args[1].asNumber().toFloat();
            p->z = args[2].asNumber().toFloat();
            p->dim = args[3].asNumber().toInt32();
            return p;
        }
        catch(...)
        {
            return nullptr;
        }
	}

    Local<Value> getX() { return Number::newNumber(x); }
    Local<Value> getY() { return Number::newNumber(y); }
    Local<Value> getZ() { return Number::newNumber(z); }
    Local<Value> getDim() { return Number::newNumber(dim); }
    void setX(const Local<Value>& value) { x = value.asNumber().toInt64(); }
    void setY(const Local<Value>& value) { y = value.asNumber().toInt64(); }
    void setZ(const Local<Value>& value) { z = value.asNumber().toInt64(); }
    void setDim(const Local<Value>& value) { dim = value.asNumber().toInt32(); }
};


//////////////////// APIs ////////////////////

Local<Value> Runcmd(const Arguments& args);
Local<Value> RuncmdEx(const Arguments& args);
Local<Value> RegisterCmd(const Arguments& args);

Local<Value> SetServerMotd(const Arguments& args);

Local<Value> Log(const Arguments& args);
Local<Value> GetTimeStr(const Arguments& args);
Local<Value> GetTimeObj(const Arguments& args);
Local<Value> RandomGuid(const Arguments& args);
Local<Value> GetLxlVersion(const Arguments& args);

Local<Value> SetTimeout(const Arguments& args);
Local<Value> SetInterval(const Arguments& args);
Local<Value> ClearInterval(const Arguments& args);