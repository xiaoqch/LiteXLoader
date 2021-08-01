#include "APIHelp.h"
#include "NbtAPI.h"
#include <Kernel/NBT.h>
#include <vector>
#include <string>
#include <LiteXLoader/API/ItemAPI.h>
#include <LiteXLoader/API/BlockAPI.h>
using namespace script;
using namespace std;


//////////////////// Class Definition ////////////////////

ClassDefine<void> NbtStaticBuilder =
    defineClass("NBT")
        .function("createTag", &NbtStatic::createTag)
        .property("End", &NbtStatic::getType<TagType::End>)
        .property("Byte", &NbtStatic::getType<TagType::Byte>)
        .property("Short", &NbtStatic::getType<TagType::Short>)
        .property("Int", &NbtStatic::getType<TagType::Int>)
        .property("Long", &NbtStatic::getType<TagType::Long>)
        .property("Float", &NbtStatic::getType<TagType::Float>)
        .property("Double", &NbtStatic::getType<TagType::Double>)
        .property("ByteArray", &NbtStatic::getType<TagType::ByteArray>)
        .property("String", &NbtStatic::getType<TagType::String>)
        .property("List", &NbtStatic::getType<TagType::List>)
        .property("Compound", &NbtStatic::getType<TagType::Compound>)
        .build();

ClassDefine<NbtValue> NbtValueBuilder =
    defineClass<NbtValue>("LXL_NbtValue")
        .constructor(nullptr)
        .instanceFunction("getType", &NbtValue::getType)
        .instanceFunction("toString", &NbtValue::toString)
        .instanceFunction("set", &NbtValue::set)
        .instanceFunction("get",&NbtValue::get)
        .build();

ClassDefine<NbtList> NbtListBuilder =
    defineClass<NbtList>("LXL_NbtList")
        .constructor(nullptr)
        .instanceFunction("getType", &NbtValue::getType)
        .instanceFunction("toString", &NbtValue::toString)
        .instanceFunction("getSize", &NbtList::getSize)
        .instanceFunction("getTypeOf", &NbtList::getTypeOf)
        .instanceFunction("setEnd", &NbtList::setEnd)
        .instanceFunction("setByte", &NbtList::setByte)
        .instanceFunction("setInt", &NbtList::setInt)
        .instanceFunction("setShort", &NbtList::setShort)
        .instanceFunction("setLong", &NbtList::setLong)
        .instanceFunction("setFloat", &NbtList::setFloat)
        .instanceFunction("setDouble", &NbtList::setDouble)
        .instanceFunction("setString", &NbtList::setString)
        .instanceFunction("setTag", &NbtList::setTag)
        .instanceFunction("addTag", &NbtList::addTag)
        .instanceFunction("removeTag", &NbtList::removeTag)
        .instanceFunction("getData", &NbtList::getData)
        .instanceFunction("getTag", &NbtList::getTag)
        .instanceFunction("toArray", &NbtList::toArray)
        .build();

ClassDefine<NbtCompound> NbtCompoundBuilder =
    defineClass<NbtCompound>("LXL_NbtCompound")
        .constructor(nullptr)
        .instanceFunction("getType", &NbtValue::getType)
        .instanceFunction("toString", &NbtValue::toString)
        .instanceFunction("getKeys", &NbtCompound::getKeys)
        .instanceFunction("getTypeOf", &NbtCompound::getTypeOf)
        .instanceFunction("setEnd", &NbtCompound::setEnd)
        .instanceFunction("setByte", &NbtCompound::setByte)
        .instanceFunction("setInt", &NbtCompound::setInt)
        .instanceFunction("setShort", &NbtCompound::setShort)
        .instanceFunction("setLong", &NbtCompound::setLong)
        .instanceFunction("setFloat", &NbtCompound::setFloat)
        .instanceFunction("setDouble", &NbtCompound::setDouble)
        .instanceFunction("setString", &NbtCompound::setString)
        .instanceFunction("setTag", &NbtCompound::setTag)
        .instanceFunction("removeTag", &NbtCompound::removeTag)
        .instanceFunction("getData", &NbtCompound::getData)
        .instanceFunction("getTag", &NbtCompound::getTag)
        .instanceFunction("toObject", &NbtCompound::toObject)
        .build();


//////////////////// Classes NbtBase ////////////////////

Local<Value> NbtBase::getType(const Arguments& args)
{
    try {
        return Number::newNumber(nbt->getTagType());
    }
    CATCH("Fail in NBTgetType!");
}


Local<Value> NbtBase::toString(const Arguments& args)
{
    if(args.size() >= 1)
        CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    try {
        return String::newString(TagToJson(nbt, args.size() >= 1 ? args[0].toInt() : -1));
    }
    CATCH("Fail in NBTtoJson!");
}


//////////////////// Classes NbtValue ////////////////////

NbtValue::NbtValue(Tag* p) :ScriptClass(ScriptClass::ConstructFromCpp<NbtValue>{})
{
	this->nbt = p;
    this->type = (TagType)(p->getTagType());
}

Tag* NbtValue::extractNBT(Local<Value> v)
{
    if (EngineScope::currentEngine()->isInstanceOf<NbtValue>(v))
        return EngineScope::currentEngine()->getNativeInstance<NbtValue>(v)->nbt;
    else
        return nullptr;
}

Local<Object> NbtValue::newNBT(Tag* p)
{
    NbtValue *nbt = new NbtValue(p);
    return nbt->getScriptObject();
}


Local<Value> NbtValue::get(const Arguments& args)
{
    try {
        return Tag2Value(nbt);
    }
    CATCH("Fail in NbtValueRead!")
}

Local<Value> NbtValue::set(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)

    try {
        return Boolean::newBoolean(TagSetValue(nbt, args[0]));
    }
    CATCH("Fail in NbtValueWrite!")
}


//////////////////// Classes NbtList ////////////////////

NbtList::NbtList(Tag* p) :ScriptClass(ScriptClass::ConstructFromCpp<NbtList>{})
{
    this->nbt = p;
}

Tag* NbtList::extractNBT(Local<Value> v)
{
    if (EngineScope::currentEngine()->isInstanceOf<NbtList>(v))
        return EngineScope::currentEngine()->getNativeInstance<NbtList>(v)->nbt;
    else
        return nullptr;
}

Local<Object> NbtList::newNBT(Tag* p)
{
    NbtList* nbt = new NbtList(p);
    return nbt->getScriptObject();
}

Local<Value> NbtList::getSize(const Arguments& args)
{
    try
    {
        return Number::newNumber(int(nbt->asList().size()));
    }
    CATCH("Fail in NBTgetSize!")
}

Local<Value> NbtList::getTypeOf(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        return Number::newNumber(int(list[index]->getTagType()));
    }
    CATCH("Fail in NBTgetTypeOf!")
}

Local<Value> NbtList::setEnd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asByte() = 0;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetEnd!")
}

Local<Value> NbtList::setByte(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();
        auto data = char(args[1].toInt());

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asByte() = data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetByte!")
}

Local<Value> NbtList::setInt(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();
        auto data = int(args[1].toInt());

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asInt() = data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetInt!")
}

Local<Value> NbtList::setShort(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();
        auto data = short(args[1].toInt());

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asShort() = data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetShort!")
}

Local<Value> NbtList::setLong(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();
        auto data = args[1].asNumber().toInt64();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asLong() =data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetLong!")
}

Local<Value> NbtList::setFloat(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();
        auto data = args[1].asNumber().toFloat();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asFloat() = data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetFloat!")
}

Local<Value> NbtList::setDouble(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();
        auto data = args[1].asNumber().toDouble();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asDouble() = data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetDouble!")
}

Local<Value> NbtList::setString(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();
        auto data = args[1].toStr();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list[index]->asString() = data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetString!")
}

Local<Value> NbtList::setTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        auto value = NbtValue::extractNBT(args[1]);
        if (value)
        {
            list[index] = value;
            return this->getScriptObject();
        }

        auto lst = NbtList::extractNBT(args[1]);
        if (lst)
        {
            list[index] = lst;
            return this->getScriptObject();
        }

        auto compound = NbtCompound::extractNBT(args[1]);
        if (compound)
        {
            list[index] = compound;
            return this->getScriptObject();
        }

        ERROR("Unknown type! Cannot add Tag into List");
        return Local<Value>();
    }
    CATCH("Fail in NBTsetTag!");
}

Local<Value> NbtList::addTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        auto value = NbtValue::extractNBT(args[0]);
        if (value)
        {
            nbt->addValue2List(value);
            return this->getScriptObject();
        }

        auto list = NbtList::extractNBT(args[0]);
        if (list)
        {
            nbt->addValue2List(list);
            return this->getScriptObject();
        }

        auto compound = NbtCompound::extractNBT(args[0]);
        if (compound)
        {
            nbt->addValue2List(compound);
            return this->getScriptObject();
        }

        ERROR("Unknown type! Cannot add Tag into List");
        return Local<Value>();
    }
    CATCH("Fail in NBTaddTag!");
}

Local<Value> NbtList::removeTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        list.erase(list.begin() + index);
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetTag!");
}

Local<Value> NbtList::getData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        return Tag2Value(list[index]);
    }
    CATCH("Fail in NBTgetData!")
}

Local<Value> NbtList::getTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asList();
        auto index = args[0].toInt();

        if (index >= list.size() || index < 0)
        {
            ERROR("Bad Index of NBT List!");
            return Local<Value>();
        }

        Local<Value> res;
        switch (list[index]->getTagType())
        {
        case TagType::List:
            res = NbtList::newNBT(list[index]);
            break;
        case TagType::Compound:
            res = NbtCompound::newNBT(list[index]);
            break;
        case TagType::End:
        case TagType::Byte:
        case TagType::Short:
        case TagType::Int:
        case TagType::Long:
        case TagType::Float:
        case TagType::Double:
        case TagType::String:
        case TagType::ByteArray:
            res = NbtValue::newNBT(list[index]);
            break;
        default:
            ERROR("Unknown type of tag!");
            res = Local<Value>();
            break;
        }
        return res;
    }
    CATCH("Fail in NBTgetTag!");
}

Local<Value> NbtList::toArray(const Arguments& args)
{
    try
    {
        auto &list = nbt->asList();
        Local<Array> arr = Array::newArray();

        for (auto& tag : list)
        {
            arr.add(Tag2Value(tag,true));
        }
        return arr;
    }
    CATCH("Fail in NBTtoArray!");
}


//////////////////// Classes NbtCompound ////////////////////

NbtCompound::NbtCompound(Tag* p) :ScriptClass(ScriptClass::ConstructFromCpp<NbtCompound>{})
{
    this->nbt = p;
}

Tag* NbtCompound::extractNBT(Local<Value> v)
{
    if (EngineScope::currentEngine()->isInstanceOf<NbtCompound>(v))
        return EngineScope::currentEngine()->getNativeInstance<NbtCompound>(v)->nbt;
    else
        return nullptr;
}

Local<Object> NbtCompound::newNBT(Tag* p)
{
    NbtCompound* nbt = new NbtCompound(p);
    return nbt->getScriptObject();
}

Local<Value> NbtCompound::getKeys(const Arguments& args)
{
    try
    {
        Local<Array> arr = Array::newArray();
        auto &list = nbt->asCompound();
        for (auto& [k, v] : list)
        {
            arr.add(String::newString(k));
        }

        return arr;
    }
    CATCH("Fail in NBTgetKeys!")
}

Local<Value> NbtCompound::getTypeOf(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        auto &list = nbt->asCompound();
        auto key = args[0].toStr();

        return Number::newNumber(int(list.at(key).getTagType()));
    }
    catch (const out_of_range& e)
    {
        ERROR("Key no found in NBT Compound!");
        return Local<Value>();
    }
    CATCH("Fail in NBTgetTypeOf!")
}

Local<Value> NbtCompound::setEnd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        auto key = args[0].toStr();
        nbt->putByte(key, (char)0);

        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetEnd!")
}

Local<Value> NbtCompound::setByte(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto key = args[0].toStr();
        auto data = char(args[1].toInt());
        nbt->putByte(key, data);

        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetByte!")
}

Local<Value> NbtCompound::setInt(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto key = args[0].toStr();
        auto data = int(args[1].toInt());
        nbt->putInt(key, data);

        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetInt!")
}

Local<Value> NbtCompound::setShort(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto key = args[0].toStr();
        auto data = short(args[1].toInt());
        nbt->putShort(key, data);

        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetShort!")
}

Local<Value> NbtCompound::setLong(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto key = args[0].toStr();
        auto data = args[1].asNumber().toInt64();
        nbt->putLong(key, data);

        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetLong!")
}

Local<Value> NbtCompound::setFloat(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto key = args[0].toStr();
        auto data = args[1].asNumber().toFloat();
        nbt->putFloat(key, data);

        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetFloat!")
}

Local<Value> NbtCompound::setDouble(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber);

    try
    {
        auto &list = nbt->asCompound();
        auto key = args[0].toStr();
        auto data = args[1].asNumber().toDouble();

        list[key].asDouble() = data;
        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetDouble!")
}

Local<Value> NbtCompound::setString(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kString);

    try
    {
        auto key = args[0].toStr();
        auto data = args[1].toStr();
        nbt->putString(key, data);

        return this->getScriptObject();
    }
    CATCH("Fail in NBTsetString!")
}

Local<Value> NbtCompound::setTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        auto key = args[0].toStr();

        auto value = NbtValue::extractNBT(args[1]);
        if (value)
        {
            nbt->put(key, value);
            return this->getScriptObject();
        }

        auto list = NbtList::extractNBT(args[1]);
        if (list)
        {
            nbt->put(key, list);
            return this->getScriptObject();
        }

        auto compound = NbtCompound::extractNBT(args[1]);
        if (compound)
        {
            nbt->putCompound(key, compound);
            return this->getScriptObject();
        }

        ERROR("Unknown type! Cannot add Tag into List");
        return Local<Value>();
    }
    CATCH("Fail in NBTsetTag!");
}

Local<Value> NbtCompound::removeTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        auto &list = nbt->asCompound();
        auto key = args[0].toStr();

        list.erase(key);
        return this->getScriptObject();
    }
    catch (const out_of_range& e)
    {
        ERROR("Key no found in NBT Compound!");
        return Local<Value>();
    }
    CATCH("Fail in NBTremoveTag!");
}

Local<Value> NbtCompound::getData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        auto &list = nbt->asCompound();
        auto key = args[0].toStr();

        return Tag2Value(&(list.at(key)));
    }
    catch (const out_of_range& e)
    {
        ERROR("Key no found in NBT Compound!");
        return Local<Value>();
    }
    CATCH("Fail in NBTgetData!")
}

Local<Value> NbtCompound::getTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);

    try
    {
        auto &list = nbt->asCompound();
        auto key = args[0].toStr();

        Local<Value> res;
        switch (list.at(key).getTagType())
        {
        case TagType::List:
            res = NbtList::newNBT(&(list.at(key)));
            break;
        case TagType::Compound:
            res = NbtCompound::newNBT(&(list.at(key)));
            break;
        case TagType::End:
        case TagType::Byte:
        case TagType::Short:
        case TagType::Int:
        case TagType::Long:
        case TagType::Float:
        case TagType::Double:
        case TagType::String:
        case TagType::ByteArray:
            res = NbtValue::newNBT(&(list.at(key)));
            break;
        default:
            ERROR("Unknown type of tag!");
            res = Local<Value>();
            break;
        }
        return res;
    }
    catch (const out_of_range& e)
    {
        ERROR("Key no found in NBT Compound!");
        return Local<Value>();
    }
    CATCH("Fail in NBTgetTag!");
}

Local<Value> NbtCompound::toObject(const Arguments& args)
{
    try
    {
        auto &list = nbt->asCompound();
        Local<Object> obj = Object::newObject();

        for (auto& [k,v] : list)
        {
            obj.set(k, Tag2Value(&v,true));
        }
        return obj;
    }
    CATCH("Fail in NBTtoArray!");
}

//////////////////// APIs ////////////////////

Local<Value> NbtStatic::createTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber)

    try {
        auto type = args[0].toInt();

        Tag* tag;
        switch (type)
        {
        case TagType::End:
            tag = Tag::createTag(TagType::End);
            break;
        case TagType::Byte:
            tag = Tag::createTag(TagType::Byte);
            break;
        case TagType::Short:
            tag = Tag::createTag(TagType::Short);
            break;
        case TagType::Int:
            tag = Tag::createTag(TagType::Int);
            break;
        case TagType::Long:
            tag = Tag::createTag(TagType::Long);
            break;
        case TagType::Float:
            tag = Tag::createTag(TagType::Float);
            break;
        case TagType::Double:
            tag = Tag::createTag(TagType::Double);
            break;
        case TagType::String:
            tag = Tag::createTag(TagType::String);
            break;
        case TagType::ByteArray:
            tag = Tag::createTag(TagType::ByteArray);
            break;
        case TagType::List:
            tag = Tag::createTag(TagType::List);
            break;
        case TagType::Compound:
            tag = Tag::createTag(TagType::Compound);
            break;
        default:
            ERROR("Unknown type of tag!");
            return Local<Value>();
        }

        if (!TagSetValue(tag, args[1]))
        {
            ERROR("Fail to set value of tag!");
            return Local<Value>();
        }
        return NbtValue::newNBT(tag);
    }
    CATCH("Fail in NBTwriteInt!")
}


//////////////////// Helper ////////////////////

Local<Value> Tag2Value_CompoundHelper(Tag* nbt, bool autoExpansion = false);

Local<Value> Tag2Value_ListHelper(Tag* nbt, bool autoExpansion = false)
{
    Local<Array> res = Array::newArray();

    auto& list = nbt->asList();
    for (auto& tag: list)
    {
        switch (tag->getTagType())
        {
        case TagType::End:
            res.add(Local<Value>());
            break;
        case TagType::Byte:
            res.add(Number::newNumber(tag->asByte()));
            break;
        case TagType::Short:
            res.add(Number::newNumber(tag->asShort()));
            break;
        case TagType::Int:
            res.add(Number::newNumber(tag->asInt()));
            break;
        case TagType::Long:
            res.add(Number::newNumber(tag->asLong()));
            break;
        case TagType::Float:
            res.add(Number::newNumber(tag->asFloat()));
            break;
        case TagType::Double:
            res.add(Number::newNumber(tag->asDouble()));
            break;
        case TagType::String:
            res.add(String::newString(tag->asString()));
            break;
        case TagType::ByteArray:
            res.add(String::newString(""));
            WARN("There are no symbol to read a ByteArray in BDS");
            break;
        case TagType::List:
            if (!autoExpansion)
                res.add(NbtList::newNBT(tag));
            else
                res.add(Tag2Value_ListHelper(tag, autoExpansion));
            break;
        case TagType::Compound:
            if (!autoExpansion)
                res.add(NbtCompound::newNBT(tag));
            else
                res.add(Tag2Value_CompoundHelper(tag, autoExpansion));
        default:
            ERROR("Unknown type of tag!");
            res.add(Local<Value>());
            break;
        }
    }
    return res;
}

Local<Value> Tag2Value_CompoundHelper(Tag* nbt, bool autoExpansion)
{
    Local<Object> res = Object::newObject();

    auto& list = nbt->asCompound();
    for (auto& [key, tag] : list)
    {
        switch (tag.getTagType())
        {
        case TagType::End:
            res.set(key, Local<Value>());
            break;
        case TagType::Byte:
            res.set(key, Number::newNumber(tag.asByte()));
            break;
        case TagType::Short:
            res.set(key, Number::newNumber(tag.asShort()));
            break;
        case TagType::Int:
            res.set(key, Number::newNumber(tag.asInt()));
            break;
        case TagType::Long:
            res.set(key, Number::newNumber(tag.asLong()));
            break;
        case TagType::Float:
            res.set(key, Number::newNumber(tag.asFloat()));
            break;
        case TagType::Double:
            res.set(key, Number::newNumber(tag.asDouble()));
            break;
        case TagType::String:
            res.set(key, String::newString(tag.asString()));
            break;
        case TagType::ByteArray:
            res.set(key, String::newString(""));
            WARN("There are no symbol to read a ByteArray in BDS");
            break;
        case TagType::List:
            if(!autoExpansion)
                res.set(key, NbtList::newNBT(&tag));
            else
                res.set(key, Tag2Value_ListHelper(&tag,autoExpansion));
            break;
        case TagType::Compound:
            if (!autoExpansion)
                res.set(key, NbtCompound::newNBT(&tag));
            else
                res.set(key, Tag2Value_CompoundHelper(&tag, autoExpansion));
        default:
            ERROR("Unknown type of tag!");
            res.set(key, Local<Value>());
            break;
        }
    }
    return res;
}

Local<Value> Tag2Value(Tag* nbt, bool autoExpansion)
{
    Local<Value> value;

    switch (nbt->getTagType())
    {
    case TagType::End:
        value = Number::newNumber(0);
        break;
    case TagType::Byte:
        value = Number::newNumber(nbt->asByte());
        break;
    case TagType::Short:
        value = Number::newNumber(nbt->asShort());
        break;
    case TagType::Int:
        value = Number::newNumber(nbt->asInt());
        break;
    case TagType::Long:
        value = Number::newNumber(nbt->asLong());
        break;
    case TagType::Float:
        value = Number::newNumber(nbt->asFloat());
        break;
    case TagType::Double:
        value = Number::newNumber(nbt->asDouble());
        break;
    case TagType::String:
        value = String::newString(nbt->asString());
        break;
    case TagType::ByteArray:
        value = ByteBuffer::newByteBuffer(0);
        WARN("There are no symbol to read a ByteArray in BDS");
        break;
    case TagType::List:
        if(!autoExpansion)
            value = NbtList::newNBT(nbt);
        else
            value = Tag2Value_ListHelper(nbt, autoExpansion);
        break;
    case TagType::Compound:
        if (!autoExpansion)
            value = NbtCompound::newNBT(nbt);
        else
            value = Tag2Value_CompoundHelper(nbt, autoExpansion);
        break;
    default:
        ERROR("Unknown type of tag!");
        value = Local<Value>();
        break;
    }
    return value;
}

bool TagSetValue(Tag* nbt, Local<Value> value)
{
    bool res = true;

    switch (nbt->getTagType())
    {
    case TagType::End:
        nbt->asByte() = 0;
        break;
    case TagType::Byte:
        nbt->asByte() = (char)(value.asNumber().toInt32());
        break;
    case TagType::Short:
        nbt->asShort() = (short)(value.asNumber().toInt32());
        break;
    case TagType::Int:
        nbt->asInt() = (int)(value.asNumber().toInt32());
        break;
    case TagType::Long:
        nbt->asLong() = value.asNumber().toInt64();
        break;
    case TagType::Float:
        nbt->asFloat() = value.asNumber().toFloat();
        break;
    case TagType::Double:
        nbt->asDouble() = value.asNumber().toDouble();
        break;
    case TagType::String:
        nbt->asString() = value.toStr();
        break;
    case TagType::ByteArray:
        WARN("There are no symbol to write a ByteArray in BDS");
        res = false;
        break;
    case TagType::List: {
        auto tag = NbtList::extractNBT(value);
        if (tag == nullptr)
        {
            ERROR("Unmatch type of tag!");
            return false;
        }
        nbt->asList() = tag->asList();
        break;
    }
    case TagType::Compound: {
        auto tag = NbtCompound::extractNBT(value);
        if (tag == nullptr)
        {
            ERROR("Unmatch type of tag!");
            return false;
        }
        nbt->asCompound() = tag->asCompound();
        break;
    }
    default:
        ERROR("Unknown type of tag!");
        return false;
        break;
    }
    return res;
}