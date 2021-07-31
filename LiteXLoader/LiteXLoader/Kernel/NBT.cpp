#include "Global.h"
#include "NBT.h"
#include <vector>
#include <map>
using namespace std;


char& Tag::asByte()
{
	return *(char*)((uintptr_t)this + 8);
}

short& Tag::asShort()
{
	return *(short*)((uintptr_t)this + 8);
}

int& Tag::asInt()
{
	return *(int*)((uintptr_t)this + 8);
}

__int64& Tag::asLong()
{
	return *(__int64*)((uintptr_t)this + 8);
}

float& Tag::asFloat()
{
	return *(float*)((uintptr_t)this + 8);
}

double& Tag::asDouble()
{
	return *(double*)((uintptr_t)this + 8);
}

std::string& Tag::asString()
{
	return *(string*)((uintptr_t)this + 8);
}

vector<Tag*> &Tag::asList()
{
	return *(vector<Tag*>*)((uintptr_t)this + 8);
}

map<string, Tag> &Tag::asCompound()
{
	return *(map<string,Tag>*)((uintptr_t)this + 8);
}

Tag* Tag::createTag(TagType t)
{
	Tag* tag = 0;
	SymCall("?newTag@Tag@@SA?AV?$unique_ptr@VTag@@U?$default_delete@VTag@@@std@@@std@@W4Type@1@@Z",
		void, Tag**, TagType)(&tag, t);
	return tag;
}

char Tag::getTagType() {
    __try
    {
        return ((*(__int64(__fastcall**)(const struct Tag*))(*(uintptr_t*)this + 40i64))(this));
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return -1;
    }
}

void Tag::put(string k, Tag* v) {
	return SymCall("?put@CompoundTag@@QEAAAEAVTag@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$$QEAV2@@Z"
		, void, void*, string, Tag*)(this, k, v);
}

void Tag::putShort(string k, short v) {
	return SymCall("?putShort@CompoundTag@@QEAAAEAFV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@F@Z"
		, void, void*, string, short)(this, k, v);
}

void Tag::putString(string k, string v) {
	return SymCall("?putString@CompoundTag@@QEAAAEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V23@0@Z"
		, void, void*, string, string)(this, k, v);
}

void Tag::putInt(string k, int v) {
	return SymCall("?putInt@CompoundTag@@QEAAAEAHV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z"
		, void, void*, string, int)(this, k, v);
}

void Tag::putLong(string k, __int64 v) {
	return SymCall("?putInt64@CompoundTag@@QEAAAEA_JV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J@Z",
		void, void*, string, __int64)(this, k, v);
}

void Tag::putFloat(string k, float v) {
	return SymCall("?putFloat@CompoundTag@@QEAAAEAMV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@M@Z"
		, void, void*, string, float)(this, k, v);
}

void Tag::putCompound(string k, Tag* v) {
    return SymCall("?putCompound@CompoundTag@@QEAAAEAV1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V1@@Z",
        void, void*, string, Tag*)(this, k, v);
}

void Tag::putByte(string k, char v) {
	return SymCall("?putByte@CompoundTag@@QEAAAEAEV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@E@Z"
		, void, void*, string, char)(this, k, v);
}

void Tag::addValue2List(Tag* t) {
	SymCall("?add@ListTag@@QEAAXV?$unique_ptr@VTag@@U?$default_delete@VTag@@@std@@@std@@@Z",
		void, void*, Tag**)(this, &t);
}

Tag* Tag::fromItem(ItemStack* item) {
	Tag* tmp = 0;
	SymCall("?save@ItemStackBase@@QEBA?AV?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@std@@XZ",
		void*, void*, Tag**)(item, &tmp);
	return tmp;
}

void Tag::setItem(ItemStack* item) {
	SymCall("?fromTag@ItemStack@@SA?AV1@AEBVCompoundTag@@@Z",
		void*, void*, Tag*)(item, this);
}

Tag* Tag::fromBlock(Block* blk) {
	return (Tag*)((uintptr_t)blk + 96);
}

void Tag::setBlock(Block* blk) {
	*(Tag*)((uintptr_t)blk + 96) = *this;
}

Tag* Tag::fromActor(Actor* actor) {
    Tag* tmp = Tag::createTag(TagType::Compound);
    SymCall("?save@Actor@@UEAA_NAEAVCompoundTag@@@Z",
        char, Actor*, Tag*)(actor, tmp);
    return tmp;
}

void Tag::setActor(Actor* actor)
{
    return;
}

//toJson

void TagToJson_Compound_Helper(JSON_VALUE& res, Tag* nbt);

void TagToJson_List_Helper(JSON_VALUE& res, Tag* nbt)
{
    auto &list = nbt->asList();
    for (auto &tag : list)
    {
        switch (tag->getTagType())
        {
        case TagType::End:
            res.push_back(nullptr);
            break;
        case TagType::Byte:
            res.push_back(tag->asByte());
            break;
        case TagType::Short:
            res.push_back(tag->asShort());
            break;
        case TagType::Int:
            res.push_back(tag->asInt());
            break;
        case TagType::Long:
            res.push_back(tag->asLong());
            break;
        case TagType::Float:
            res.push_back(tag->asFloat());
            break;
        case TagType::Double:
            res.push_back(tag->asDouble());
            break;
        case TagType::String:
            res.push_back(tag->asString());
            break;
        case TagType::ByteArray:
            res.push_back("");
            WARN("There are no symbol to read a ByteArray in BDS");
            break;
        case TagType::List: {
            JSON_VALUE arrJson = JSON_VALUE::array();
            TagToJson_List_Helper(arrJson, tag);
            res.push_back(arrJson);
            break;
        }
        case TagType::Compound: {
            JSON_VALUE arrObj = JSON_VALUE::object();
            TagToJson_Compound_Helper(arrObj, tag);
            res.push_back(arrObj);
            break;
        }
        default:
            res.push_back(nullptr);
            break;
        }
    }
}

void TagToJson_Compound_Helper(JSON_VALUE& res, Tag* nbt)
{
    auto &list = nbt->asCompound();
    for (auto& [key,tag] : list)
    {
        switch (tag.getTagType())
        {
        case TagType::End:
            res.push_back({ key,nullptr });
            break;
        case TagType::Byte:
            res.push_back({ key,tag.asByte() });
            break;
        case TagType::Short:
            res.push_back({ key,tag.asShort() });
            break;
        case TagType::Int:
            res.push_back({ key,tag.asInt() });
            break;
        case TagType::Long:
            res.push_back({ key,tag.asLong() });
            break;
        case TagType::Float:
            res.push_back({ key,tag.asFloat() });
            break;
        case TagType::Double:
            res.push_back({ key,tag.asDouble() });
            break;
        case TagType::String:
            res.push_back({ key,tag.asString() });
            break;
        case TagType::ByteArray:
            res.push_back({ key,"" });
            WARN("There are no symbol to read a ByteArray in BDS");
            break;
        case TagType::List: {
            JSON_VALUE arrJson = JSON_VALUE::array();
            TagToJson_List_Helper(arrJson, &tag);
            res.push_back({ key,arrJson });
            break;
        }
        case TagType::Compound: {
            JSON_VALUE arrObj = JSON_VALUE::object();
            TagToJson_Compound_Helper(arrObj, &tag);
            res.push_back({ key,arrObj });
            break;
        }
        default:
            res.push_back({ key,nullptr });
            break;
        }
    }
}

string TagToJson(Tag* nbt, int formatIndent)
{
    string result;
    switch (nbt->getTagType())
    {
    case TagType::End:
        result = "";
        break;
    case TagType::Byte:
        result = to_string(nbt->asByte());
        break;
    case TagType::Short:
        result = to_string(nbt->asShort());
        break;
    case TagType::Int:
        result = to_string(nbt->asInt());
        break;
    case TagType::Long:
        result = to_string(nbt->asLong());
        break;
    case TagType::Float:
        result = to_string(nbt->asFloat());
        break;
    case TagType::Double:
        result = to_string(nbt->asDouble());
        break;
    case TagType::String:
        result = nbt->asString();
        break;
    case TagType::ByteArray:
        result = "";
        WARN("There are no symbol to read a ByteArray in BDS");
        break;
    case TagType::List: {
        JSON_VALUE jsonRes = JSON_VALUE::array();
        TagToJson_List_Helper(jsonRes, nbt);
        result = jsonRes.dump(formatIndent);
        break;
    }
    case TagType::Compound: {
        JSON_VALUE jsonRes = JSON_VALUE::object();
        TagToJson_Compound_Helper(jsonRes, nbt);
        result = jsonRes.dump(formatIndent);
        break;
    }
    default:
        result = "";
        break;
    }
    return result;
}