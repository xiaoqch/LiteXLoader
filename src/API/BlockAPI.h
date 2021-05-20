#pragma once
#include <string>
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

public:
	explicit BlockClass(Block *p);

    Block *get()
    {
        return block;
    }

    static Local<Object> newBlock(Block *p)
    {
        auto newp = new BlockClass(p);
        return newp->getScriptObject();
    }
    static Local<Object> newBlock(WBlock p)
    {
        return BlockClass::newBlock(p.v);
    }
    static Block* extractBlock(Local<Value> v)
    {
        if(EngineScope::currentEngine()->isInstanceOf<BlockClass>(v))
            return EngineScope::currentEngine()->getNativeInstance<BlockClass>(v)->get();
        else
            return nullptr;
    }

    Local<Value> getName();
};