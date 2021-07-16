#include "APIhelp.h"
#include "NbtAPI.h"
#include <Kernel/NBT.h>
using namespace script;

ClassDefine<NBTClass> NBTClassBuilder =
defineClass<NBTClass>("NBT")
.constructor(nullptr)
.instanceFunction("readInt", &NBTClass::readInt)
.instanceFunction("readLong", &NBTClass::readLong)
.instanceFunction("readString", &NBTClass::readString)
.instanceFunction("readFloat", &NBTClass::readFloat)
.instanceFunction("readDouble", &NBTClass::readDouble)
.instanceFunction("readBoolean", &NBTClass::readBoolean)
.instanceFunction("readByte", &NBTClass::readByte)
.instanceFunction("readList", &NBTClass::readList)
.instanceFunction("readInt", &NBTClass::readCompound)
.instanceFunction("getType", &NBTClass::getType)
.build();

NBTClass::NBTClass(Tag* p) :ScriptClass(ScriptClass::ConstructFromCpp<NBTClass>{}), nbt(p) {
	this->nbt = p;
}

Tag* NBTClass::extractNBT(Local<Value> v) {
    if (EngineScope::currentEngine()->isInstanceOf<NBTClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<NBTClass>(v)->nbt;
    else
        return nullptr;
}

Local<Object> NBTClass::newNBT(Tag* p)
{
    auto tmp = new NBTClass(p);
    return tmp->getScriptObject();
}

Local<Value> NBTClass::readInt()
{
    try {
        auto v = nbt->getValue<int>();
        return Number::newNumber(v);
    }
    CATCH("Fail in NBTreadInt!")
}

Local<Value> NBTClass::readLong()
{
    try {
        auto v = nbt->getValue<__int64>();
        return Number::newNumber(v);
    }
    CATCH("Fail in NBTreadLong!")
}

Local<Value> NBTClass::readFloat()
{
    try {
        auto v = nbt->getValue<float>();
        return Number::newNumber(v);
    }
    CATCH("Fail in NBTreadFloat!")
}

Local<Value> NBTClass::readDouble()
{
    try {
        auto v = nbt->getValue<double>();
        return Number::newNumber(v);
    }
    CATCH("Fail in NBTreadDouble!")
}

Local<Value> NBTClass::readBoolean()
{
    try {
        auto v = nbt->getValue<char>();
        return Boolean::newBoolean(v);
    }
    CATCH("Fail in NBTreadBoolean!")
}

Local<Value> NBTClass::readString()
{
    try {
        auto v = nbt->getValue<string>();
        return String::newString(v);
    }
    CATCH("Fail in NBTreadString!")
}

Local<Value> NBTClass::readList()
{
    try {
        auto v = nbt->getValue<std::vector<Tag*>>();
        auto arr = Array::newArray();
        for (auto i : v) {
            auto tmp = NBTClass::newNBT(i);
            arr.add(tmp);
        }
        return arr;
    }
    CATCH("Fail in NBTreadList!")
}

Local<Value> NBTClass::readCompound()
{
    try {
        auto v = nbt->getValue<std::map<string, Tag>>();
        auto obj = Object::newObject();
        for (auto i : v) {
            auto k = String::newString(i.first);
            auto v = NBTClass::newNBT(&(i.second));
            obj.set(k, v);
        }
        return obj;
    }
    CATCH("Fail in NBTreadCompound!")
}


Local<Value> NBTClass::readByte()
{
    try {
        auto v = nbt->getValue<char>();
        return Number::newNumber(v);
    }
    CATCH("Fail in NBTreadByte!")
}

Local<Value> NBTClass::getType()
{
    auto type = nbt->getTagType();
    switch (type)
    {
    case TagType::End:
        return String::newString("end");
    case TagType::Byte:
        return String::newString("Byte/Boolean");
    case TagType::Short:
        return String::newString("Short");
    case TagType::Int:
        return String::newString("Int");
    case TagType::Int64:
        return String::newString("Long");
    case TagType::Float:
        return String::newString("Float");
    case TagType::Double:
        return String::newString("Double");
    case TagType::String:
        return String::newString("String");
    case TagType::ByteArray:
        return String::newString("ByteArray");
    case TagType::List:
        return String::newString("List");
    case TagType::Compound:
        return String::newString("Compound");
    }
}
