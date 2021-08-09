#pragma once
#include "Global.h"
#include "Data.h"
#include <vector>
#include <string>
#include <map>

enum TagType : uint8_t {
	End, Byte, Short, Int, Long, Float, Double, ByteArray, String, List, Compound,
};

class Player;
class Actor;
class Block;
class ItemStack;

struct TagMemoryChunk {
	size_t capacity = 0;
	size_t size = 0;
	unique_ptr<uint8_t[]> data;

	TagMemoryChunk(size_t size, uint8_t data[]) :capacity(size), size(size), data(data) {}
};

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
	TagMemoryChunk& asByteArray();

	//share
	static Tag* createTag(TagType t);
	char getTagType();

	//compound
	void put(const std::string& k, Tag* v);
	void putShort(const std::string& k, short v);
	void putString(const std::string& k, std::string v);
	void putInt(const std::string& k, int v);
	void putLong(const std::string& k, __int64 v);
	void putFloat(const std::string& k, float v);
	void putCompound(const std::string& k, Tag* v);
	void putByte(const std::string& k, char v);
	void putByteArray(const std::string& key, const TagMemoryChunk& value);
	void destroy();

	//list
	void addValue2List(Tag* t);

	//static
	void setItem(ItemStack* item);
	void setBlock(Block* blk);
	bool setActor(Actor* actor);
	bool setPlayer(Player* actor);
	static Tag* fromItem(ItemStack* item);
	static Tag* fromBlock(Block* blk);
	static Tag* fromActor(Actor* actor);
	static Tag* fromPlayer(Player* player);
};

std::string TagToJson(Tag* nbt, int formatIndent);