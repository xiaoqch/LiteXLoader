#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;
#include <Kernel/Global.h>
#include <string>

//////////////////// Classes ////////////////////
class IntPos : public IntVec4, public ScriptClass
{
public:
	explicit IntPos(const Local<Object>& scriptObj) : ScriptClass(scriptObj) {}
    static IntPos* create(const Arguments& args);

    static Local<Object> newPos(int x, int y, int z, int dim=-1);
    static Local<Object> newPos(const BlockPos &b, int dim=-1);
    static Local<Object> newPos(const IntVec4 &v);
    static IntPos* extractPos(Local<Value> v);

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
    static FloatPos* create(const Arguments& args);

    static Local<Object> newPos(double x, double y, double z, int dim=-1);
    static Local<Object> newPos(const Vec3 &v, int dim=-1);
    static Local<Object> newPos(const FloatVec4 &v);
    static FloatPos* extractPos(Local<Value> v);

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
Local<Value> RegisterPlayerCmd(const Arguments& args);
Local<Value> RegisterConsoleCmd(const Arguments& args);

Local<Value> Log(const Arguments& args);
Local<Value> GetTimeStr(const Arguments& args);
Local<Value> GetTimeObj(const Arguments& args);
Local<Value> RandomGuid(const Arguments& args);

Local<Value> SetTimeout(const Arguments& args);
Local<Value> SetInterval(const Arguments& args);
Local<Value> ClearInterval(const Arguments& args);