#pragma once
#include <ScriptX/ScriptX.h>
#include <Kernel/NBT.h>
using namespace script;

extern struct Tag;

class NbtBase
{
protected:
    Tag* nbt;

public:
    Local<Value> getType(const Arguments& args);
    Local<Value> toString(const Arguments& args);
};


//NBT Static
class NbtStatic : public ScriptClass
{
public:
    static Local<Value> createTag(const Arguments& args);

    template<TagType T>
    static Local<Value> getType()
    {
        return Number::newNumber((int)T);
    }
};
extern ClassDefine<void> NbtStaticBuilder;


//NBT Value
class NbtValue : public NbtBase, public ScriptClass
{
private:
    TagType type;

public:
    explicit NbtValue(Tag* p);

    static Tag* extractNBT(Local<Value> v);
    static Local<Object> newNBT(Tag* p);
    TagType getValueType() { return type; }

    Local<Value> set(const Arguments& args);
    Local<Value> get(const Arguments& args);
};


//NBT List
class NbtList : public NbtBase, public ScriptClass
{
public:
    explicit NbtList(Tag* p);

    static Tag* extractNBT(Local<Value> v);
    static Local<Object> newNBT(Tag* p);

    Local<Value> getSize(const Arguments& args);
    Local<Value> getTypeOf(const Arguments& args);

    Local<Value> setEnd(const Arguments& args);
    Local<Value> setByte(const Arguments& args);
    Local<Value> setInt(const Arguments& args);
    Local<Value> setShort(const Arguments& args);
    Local<Value> setLong(const Arguments& args);
    Local<Value> setFloat(const Arguments& args);
    Local<Value> setDouble(const Arguments& args);
    Local<Value> setString(const Arguments& args);
    Local<Value> setTag(const Arguments& args);
    Local<Value> addTag(const Arguments& args);
    Local<Value> removeTag(const Arguments& args);

    Local<Value> getData(const Arguments& args);
    Local<Value> getTag(const Arguments& args);

    Local<Value> toArray(const Arguments& args);
};


//NBT Compound
class NbtCompound : public NbtBase, public ScriptClass
{
public:
    explicit NbtCompound(Tag* p);

    static Tag* extractNBT(Local<Value> v);
    static Local<Object> newNBT(Tag* p);

    Local<Value> getKeys(const Arguments& args);
    Local<Value> getTypeOf(const Arguments& args);

    Local<Value> setEnd(const Arguments& args);
    Local<Value> setByte(const Arguments& args);
    Local<Value> setInt(const Arguments& args);
    Local<Value> setShort(const Arguments& args);
    Local<Value> setLong(const Arguments& args);
    Local<Value> setFloat(const Arguments& args);
    Local<Value> setDouble(const Arguments& args);
    Local<Value> setString(const Arguments& args);
    Local<Value> setTag(const Arguments& args);
    Local<Value> removeTag(const Arguments& args);

    Local<Value> getData(const Arguments& args);
    Local<Value> getTag(const Arguments& args);

    Local<Value> toObject(const Arguments& args);
};


//Helper
Local<Value> Tag2Value(Tag* nbt, bool autoExpansion = false);
bool TagSetValue(Tag* nbt, Local<Value> value);