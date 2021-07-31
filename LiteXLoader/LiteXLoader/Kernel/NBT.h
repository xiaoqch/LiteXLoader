#pragma once
#include "Global.h"
#include "Data.h"
#include <vector>
#include <string>
#include <map>

enum TagType {
	End, Byte, Short, Int, Long, Float, Double, ByteArray, String, List, Compound,
};

class Player;
class Actor;
class Block;
class ItemStack;

class Tag {
private:
	char filler[0x28];
public:
	//value
	char&		asByte();
	short&		asShort();
	int&		asInt();
	__int64&	asLong();
	float&		asFloat();
	double&		asDouble();
	std::string& asString();
	std::vector<Tag*> &asList();
	std::map<std::string, Tag> &asCompound();

	//share
	static Tag* createTag(TagType t);
	char getTagType();

	//compound
	void put(std::string k, Tag* v);
	void putShort(std::string k, short v);
	void putString(std::string k, std::string v);
	void putInt(std::string k, int v);
	void putLong(std::string k, __int64 v);
	void putFloat(std::string k, float v);
	void putCompound(std::string k, Tag* v);
	void putByte(std::string k, char v);

	//list
	void addValue2List(Tag* t);

	//static
	void setItem(ItemStack* item);
	void setBlock(Block* blk);
	void setActor(Actor* actor);
	static Tag* fromItem(ItemStack* item);
	static Tag* fromBlock(Block* blk);
	static Tag* fromActor(Actor* actor);
};

std::string TagToJson(Tag* nbt, int formatIndent);