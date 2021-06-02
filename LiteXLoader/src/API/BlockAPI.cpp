#include "APIhelp.h"
#include "BlockAPI.h"
#include <Kernel/Block.h>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<BlockClass> BlockClassBuilder =
    defineClass<BlockClass>("Block")
        .constructor(nullptr)
        .instanceProperty("name", &BlockClass::getName)
        .instanceProperty("pos", &BlockClass::getPos)
        .build();


//////////////////// Classes ////////////////////

BlockClass::BlockClass(Block *p)
    :ScriptClass(ScriptClass::ConstructFromCpp<BlockClass>{}),block(p)
{
    name = Raw_GetBlockFullName(block);
    customName = Raw_GetBlockName(block);
    pos = {0,0,0,-1};
}

BlockClass::BlockClass(Block *p, BlockPos bp, int dim)
    :ScriptClass(ScriptClass::ConstructFromCpp<BlockClass>{}),block(p)
{
    name = Raw_GetBlockFullName(block);
    customName = Raw_GetBlockName(block);
    pos = {bp.x,bp.y,bp.z,dim};
}

//生成函数
Local<Object> BlockClass::newBlock(Block *p)
{
    auto newp = new BlockClass(p);
    return newp->getScriptObject();
}
Local<Object> BlockClass::newBlock(Block *p, BlockPos *pos)
{
    auto newp = new BlockClass(p,*pos,-1);
    return newp->getScriptObject();
}
Local<Object> BlockClass::newBlock(Block *p, BlockPos *pos, int dim)
{
    auto newp = new BlockClass(p,*pos,dim);
    return newp->getScriptObject();
}
Local<Object> BlockClass::newBlock(Block *p, BlockPos *pos, BlockSource *bs)
{
    auto newp = new BlockClass(p,*pos,Raw_GetBlockDimension(bs));
    return newp->getScriptObject();
}
Local<Object> BlockClass::newBlock(WBlock p)
{
    return BlockClass::newBlock(p.v);
}
Block* BlockClass::extractBlock(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<BlockClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<BlockClass>(v)->get();
    else
        return nullptr;
}

//成员函数
Local<Value> BlockClass::getName()
{
    try{
        //return String::newString(Raw_GetBlockName(block));
        return String::newString(name);
    }
    CATCH("Fail in GetBlockName!")
}

Local<Value> BlockClass::getCustomName()
{
    try{
        //return String::newString(Raw_GetBlockName(block));
        return String::newString(customName);
    }
    CATCH("Fail in GetBlockName!")
}

Local<Value> BlockClass::getPos()
{
    try{
        return IntPos::newPos(pos);
    }
    CATCH("Fail in GetBlockPos!")
}

//公用API
Local<Value> GetBlock(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    try{
        auto pos = IntPos::extractPos(args[0]);
        if(!pos)
            return Local<Value>();    //Null
        else
        {
            auto block = Raw_GetBlockByPos(pos);
            if(!block)
                return Local<Value>();    //Null
            else
                return BlockClass::newBlock(block);
        }
    }
    CATCH("Fail in GetBlock!")
}