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

    Local<Value> readInt();
    Local<Value> readLong();
    Local<Value> readFloat();
    Local<Value> readDouble();
    Local<Value> readBoolean();
    Local<Value> readString();
    Local<Value> readList();
    Local<Value> readCompound();
    Local<Value> readByte();
    Local<Value> getType();
};
