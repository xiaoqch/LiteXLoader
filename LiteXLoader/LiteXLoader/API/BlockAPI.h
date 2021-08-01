#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

#include <Kernel/Global.h>
#include <string>


//////////////////// Classes ////////////////////
class Block;
class BlockClass : public ScriptClass
{
private:
    Block *block;

    // Pre data
    std::string name,type;
    int id;
    IntVec4 pos;

public:
	explicit BlockClass(Block *p);
    BlockClass(Block *p, BlockPos bp, int dim);

    void preloadData(BlockPos bp, int dim);
    Block *get()
    {
        return block;
    }


    static Local<Object> newBlock(Block *p);
    static Local<Object> newBlock(Block *p, BlockPos *pos);
    static Local<Object> newBlock(Block *p, BlockPos *pos, int dim);
    static Local<Object> newBlock(Block *p, BlockPos *pos, BlockSource *bs);
    static Local<Object> newBlock(WBlock p);
    static Block* extractBlock(Local<Value> v);
    Local<Value> getRawPtr(const Arguments& args);

    Local<Value> getName();
    Local<Value> getType();
    Local<Value> getId();
    Local<Value> getPos();

    Local<Value> getTag(const Arguments& args);
    Local<Value> setTag(const Arguments& args);
    Local<Value> getBlockState(const Arguments& args);
};

//////////////////// APIs ////////////////////

Local<Value> GetBlock(const Arguments& args);
Local<Value> SetBlock(const Arguments& args);
Local<Value> SpawnParticle(const Arguments& args);