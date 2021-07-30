#pragma once
#include "Global.h"
#include "Data.h"
#include <vector>
#include <string>
#include <map>

enum TagType {
	End, Byte, Short, Int, Long, Float, Double, ByteArray, String, List, Compound,
};

class Tag {
private:
	char filler[0x28];
public:

	char&		asByte();
	short&		asShort();
	int&		asInt();
	__int64&	asLong();
	float&		asFloat();
	double&		asDouble();
	std::string& asString();
	std::vector<Tag*> &asList();
	std::map<std::string, Tag*> &asCompound();

	static Tag* createTag(TagType t);
	char getTagType();

	void put(string k, Tag* v);
	void put(string k, short v);
	void put(string k, string v);
	void put(string k, int v);
	void put(string k, __int64 v);
	void put(string k, float v);
	void put(string k, Tag** v);
	void put(string k, char v);

	void addValue2List(Tag* t);

	void setItem(ItemStack* item);
	void setBlock(Block* blk);
	void setActor(Actor* actor);
	static Tag* fromItem(ItemStack* item);
	static Tag* fromBlock(Block* blk);
	static Tag* fromActor(Actor* actor);
};

std::string TagToJson(Tag* nbt, int formatIndent);