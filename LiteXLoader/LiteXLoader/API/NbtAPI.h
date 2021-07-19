#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

extern class Tag;
class NBTClass : public ScriptClass
{
private:
    Tag* nbt;
public:
    explicit NBTClass(Tag* p);

    static Tag* extractNBT(Local<Value> v);
    static Local<Object> newNBT(Tag* p);

    Local<Value> readInt(const Arguments& args);
    Local<Value> readLong(const Arguments& args);
    Local<Value> readFloat(const Arguments& args);
    Local<Value> readDouble(const Arguments& args);
    Local<Value> readBoolean(const Arguments& args);
    Local<Value> readString(const Arguments& args);
    Local<Value> readList(const Arguments& args);
    Local<Value> readCompound(const Arguments& args);
    Local<Value> readByte(const Arguments& args);
    Local<Value> getType(const Arguments& args);
};
