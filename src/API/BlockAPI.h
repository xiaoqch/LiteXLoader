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

    Local<Value> getName();
};