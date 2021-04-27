#pragma once
#include "ScriptX.h"
using namespace script;

//////////////////// Classes ////////////////////

class Block;
class BlockPointer : public ScriptClass
{
public:
	Block *pointer;

	explicit BlockPointer(Block *p)
        :ScriptClass(ScriptClass::ConstructFromCpp<BlockPointer>{}),pointer(p)
    {}
    Block *get()
    {
        return pointer;
    }
};