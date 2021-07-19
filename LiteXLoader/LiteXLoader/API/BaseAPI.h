#pragma once
#include <ScriptX/ScriptX.h>
#include <Kernel/pch.h>
#include <string>
using namespace script;

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
    Local<Value> getDim();
    Local<Value> getDimId() { return Number::newNumber(dim); }
    void setX(const Local<Value>& value) { x = value.asNumber().toInt64(); }
    void setY(const Local<Value>& value) { y = value.asNumber().toInt64(); }
    void setZ(const Local<Value>& value) { z = value.asNumber().toInt64(); }
    void setDimId(const Local<Value>& value) { dim = value.asNumber().toInt32(); }
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
    Local<Value> getDim();
    Local<Value> getDimId() { return Number::newNumber(dim); }
    void setX(const Local<Value>& value) { x = value.asNumber().toInt64(); }
    void setY(const Local<Value>& value) { y = value.asNumber().toInt64(); }
    void setZ(const Local<Value>& value) { z = value.asNumber().toInt64(); }
    void setDimId(const Local<Value>& value) { dim = value.asNumber().toInt32(); }
};