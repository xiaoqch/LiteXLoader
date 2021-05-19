#include "APIhelp.h"
#include "BlockAPI.h"
#include "../Kernel/Block.h"
using namespace script;

BlockClass::BlockClass(Block *p)
    :ScriptClass(ScriptClass::ConstructFromCpp<BlockClass>{}),block(p)
{
    name = Raw_GetBlockName(block);
}

Local<Value> BlockClass::getName()
{
    try{
        //return String::newString(Raw_GetBlockName(block));
        return String::newString(name);
    }
    CATCH("Fail in GetBlockName!")
}