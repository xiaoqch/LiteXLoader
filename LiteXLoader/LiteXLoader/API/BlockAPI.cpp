#include "APIHelp.h"
#include "BaseAPI.h"
#include "BlockAPI.h"
#include <Kernel/Block.h>
#include <Kernel/SymbolHelper.h>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<BlockClass> BlockClassBuilder =
    defineClass<BlockClass>("LXL_Block")
        .constructor(nullptr)
        .instanceProperty("name", &BlockClass::getName)
        .instanceProperty("type", &BlockClass::getType)
        .instanceProperty("id", &BlockClass::getId)
        .instanceProperty("pos", &BlockClass::getPos)
        .build();


//////////////////// Classes ////////////////////

BlockClass::BlockClass(Block *p)
    :ScriptClass(ScriptClass::ConstructFromCpp<BlockClass>{}),block(p)
{
    preloadData({ 0,0,0 }, -1);
}

BlockClass::BlockClass(Block *p, BlockPos bp, int dim)
    :ScriptClass(ScriptClass::ConstructFromCpp<BlockClass>{}),block(p)
{
    preloadData(bp, dim);
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
void BlockClass::preloadData(BlockPos bp, int dim)
{
    name = Raw_GetBlockName(block);
    type = Raw_GetBlockFullName(block);
    id = Raw_GetBlockId(block);
    pos = { bp.x,bp.y,bp.z,dim };
}

Local<Value> BlockClass::getName()
{
    try{
        // 已预加载
        return String::newString(name);
    }
    CATCH("Fail in getBlockName!")
}

Local<Value> BlockClass::getType()
{
    try{
        // 已预加载
        return String::newString(type);
    }
    CATCH("Fail in getBlockType!")
}

Local<Value> BlockClass::getId()
{
    try {
        // 已预加载
        return Number::newNumber(id);
    }
    CATCH("Fail in getBlockId!")
}

Local<Value> BlockClass::getPos()
{
    try{
        // 已预加载
        return IntPos::newPos(pos);
    }
    CATCH("Fail in getBlockPos!")
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
            {
                BlockPos bp{ pos->x,pos->y,pos->z };
                return BlockClass::newBlock(block, &bp, pos->dim);
            }
        }
    }
    CATCH("Fail in GetBlock!")
}

Local<Value> SetBlock(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)

    try
    {
        IntPos* pos = IntPos::extractPos(args[0]);
        if (!pos)
            return Local<Value>();
        if (pos->dim < 0)
            return Boolean::newBoolean(false);

        if (args[1].isString())
        {
            //方块名
            return Boolean::newBoolean(Raw_SetBlockByName(*pos, args[1].toStr()));
        }
        else
        {
            //其他方块对象
            Block* bl = BlockClass::extractBlock(args[1]);
            if (!bl)
                return Local<Value>();
            return Boolean::newBoolean(Raw_SetBlockByBlock(*pos, bl));
        }
    }
    CATCH("Fail in SetBlock!")
}

Local<Value> SpawnParticle(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[1], ValueKind::kString)

    try
    {
        IntPos* pos = IntPos::extractPos(args[0]);
        if (!pos)
            return Local<Value>();
        if (pos->dim < 0)
            return Boolean::newBoolean(false);

        return Boolean::newBoolean(Raw_SpawnParticle(*pos, args[1].toStr()));
    }
    CATCH("Fail in SpawnParticle!")
}