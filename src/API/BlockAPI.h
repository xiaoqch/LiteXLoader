#pragma once
#include <string>
#include "../Kernel/Global.h"
#include "ScriptX.h"
using namespace script;

//////////////////// Classes ////////////////////
class Block;
class BlockClass : public ScriptClass
{
private:
    Block *block;

    // Pre data
    std::string name;
    IntVec4 pos;

public:
	explicit BlockClass(Block *p);
    BlockClass(Block *p, BlockPos bp, int dim);

    Block *get()
    {
        return block;
    }

    static Local<Object> newBlock(Block *p);
    static Local<Object> newBlock(Block *p, BlockPos *pos);
    static Local<Object> newBlock(Block *p, BlockPos *pos, BlockSource *bs);
    static Local<Object> newBlock(WBlock p);
    static Block* extractBlock(Local<Value> v);

    Local<Value> getName();
    Local<Value> getPos();
};