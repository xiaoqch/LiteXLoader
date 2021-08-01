#include "APIHelp.h"
#include "BaseAPI.h"
#include "BlockAPI.h"
#include "NbtAPI.h"
#include <Kernel/NBT.h>
#include <Kernel/Block.h>
#include <Kernel/SymbolHelper.h>
#include <exception>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<BlockClass> BlockClassBuilder =
    defineClass<BlockClass>("LXL_Block")
        .constructor(nullptr)
        .instanceFunction("getRawPtr", &BlockClass::getRawPtr)
        
        .instanceProperty("name", &BlockClass::getName)
        .instanceProperty("type", &BlockClass::getType)
        .instanceProperty("id", &BlockClass::getId)
        .instanceProperty("pos", &BlockClass::getPos)

        .instanceFunction("setTag", &BlockClass::setTag)
        .instanceFunction("getTag", &BlockClass::getTag)
        .instanceFunction("getBlockState", &BlockClass::getBlockState)
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

Local<Value> BlockClass::getRawPtr(const Arguments& args)
{
    try {
        return Number::newNumber((intptr_t)block);
    }
    CATCH("Fail in getRawPtr!")
}

Local<Value> BlockClass::getTag(const Arguments& args)
{
    try {
        return NbtCompound::newNBT(Tag::fromBlock(block));
    }
    CATCH("Fail in getTag!")
}

Local<Value> BlockClass::setTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);

    try {
        auto nbt = NbtCompound::extractNBT(args[0]);
        if (!nbt)
            return Local<Value>();    //Null
        
        nbt->setBlock(block);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in setTag!")
}

Local<Value> BlockClass::getBlockState(const Arguments& args)
{
    try {
        auto list = Tag::fromBlock(block)->asCompound();
        return Tag2Value(&list.at("states"),true);
    }
    catch (const std::out_of_range& e)
    {
        return Object::newObject();
    }
    CATCH("Fail in getBlockState!")
}


//公用API
Local<Value> GetBlock(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);

    try{
        IntVec4 pos;
        if (args.size() == 1)
        {
            // IntPos
            auto posObj = IntPos::extractPos(args[0]);
            if (posObj)
            {
                if (posObj->dim < 0)
                    return Local<Value>();
                else
                    pos = *posObj;
            }
            else
            {
                ERROR("Wrong type of argument in GetBlock!");
                return Local<Value>();
            }
        }
        else if(args.size() == 4)
        {
            // Number Pos
            CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[2], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[3], ValueKind::kNumber);
            pos = { args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt() };
        }
        else
        {
            ERROR("Wrong number of arguments in GetBlock!");
            return Local<Value>();
        }

        auto block = Raw_GetBlockByPos(&pos);
        if (!block)
            return Local<Value>();    //Null
        else
        {
            BlockPos bp{ pos.x,pos.y,pos.z };
            return BlockClass::newBlock(block, &bp, pos.dim);
        }
    }
    CATCH("Fail in GetBlock!")
}

Local<Value> SetBlock(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);

    try
    {
        IntVec4 pos;
        Local<Value> block;

        if (args.size() == 1)
        {
            // IntPos
            IntPos* posObj = IntPos::extractPos(args[0]);
            if (posObj)
            {
                if (posObj->dim < 0)
                    return Boolean::newBoolean(false);
                else
                {
                    pos = *posObj;
                    block = args[1];
                }
            }
            else
            {
                ERROR("Wrong type of argument in SetBlock!");
                return Local<Value>();
            }
        }
        else if (args.size() == 4)
        {
            // Number Pos
            CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[2], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[3], ValueKind::kNumber);
            pos = { args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt() };
            block = args[4];
        }
        else
        {
            ERROR("Wrong number of arguments in SetBlock!");
            return Local<Value>();
        }


        if (block.isString())
        {
            //方块名
            return Boolean::newBoolean(Raw_SetBlockByName(pos, block.toStr()));
        }
        else
        {
            //其他方块对象
            Block* bl = BlockClass::extractBlock(block);
            if (!bl)
                return Local<Value>();
            return Boolean::newBoolean(Raw_SetBlockByBlock(pos, bl));
        }
    }
    CATCH("Fail in SetBlock!")
}

Local<Value> SpawnParticle(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)      

    try
    {
        IntPos* posObj = IntPos::extractPos(args[0]);
        IntVec4 pos;
        Local<Value> type;

        if (args.size() == 1)
        {
            // IntPos
            CHECK_ARG_TYPE(args[1], ValueKind::kString);

            IntPos* posObj = IntPos::extractPos(args[0]);
            if (posObj)
            {
                if (posObj->dim < 0)
                    return Boolean::newBoolean(false);
                else
                {
                    pos = *posObj;
                    type = args[1];
                }
            }
            else
            {
                ERROR("Wrong type of argument in SpawnParticle!");
                return Local<Value>();
            }
        }
        else if (args.size() == 4)
        {
            // Number Pos
            CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[2], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[3], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[4], ValueKind::kString);

            pos = { args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt() };
            type = args[4];
        }
        else
        {
            ERROR("Wrong number of arguments in SpawnParticle!");
            return Local<Value>();
        }

        return Boolean::newBoolean(Raw_SpawnParticle(pos, type.toStr()));
    }
    CATCH("Fail in SpawnParticle!")
}