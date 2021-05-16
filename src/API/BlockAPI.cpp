#include "APIhelp.h"
#include "BlockAPI.h"
#include "../Kernel/Block.h"
using namespace script;

Local<Value> GetBlockName(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Block *block = ExtractBlock(args[0]);
        if(block)
            return String::newString(Raw_GetBlockName(block));
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetBlockName!")
}