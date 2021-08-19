#include "APIHelp.h"
#include "BlockEntityAPI.h"
#include "NbtAPI.h"
#include <Kernel/Block.h>
#include <Kernel/BlockEntity.h>
#include <Kernel/Nbt.h>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<BlockEntityClass> BlockEntityClassBuilder =
	defineClass<BlockEntityClass>("LXL_BlockEntity")
		.constructor(nullptr)
		.instanceFunction("getRawPtr", &BlockEntityClass::getRawPtr)

		.instanceProperty("type", &BlockEntityClass::getType)

		.instanceFunction("setNbt", &BlockEntityClass::setNbt)
		.instanceFunction("getNbt", &BlockEntityClass::getNbt)
		.build();


//////////////////// Classes ////////////////////

BlockEntityClass::BlockEntityClass(BlockActor* be)
	:ScriptClass(ScriptClass::ConstructFromCpp<BlockEntityClass>{}),blockEntity(be)
{ }

Local<Object> BlockEntityClass::newBlockEntity(BlockActor * be)
{
	auto newp = new BlockEntityClass(be);
	return newp->getScriptObject();
}

//生成函数
BlockActor* BlockEntityClass::extractBlockEntity(Local<Value> v)
{
	if (EngineScope::currentEngine()->isInstanceOf<BlockEntityClass>(v))
		return EngineScope::currentEngine()->getNativeInstance<BlockEntityClass>(v)->get();
	else
		return nullptr;
}

//成员函数
Local<Value> BlockEntityClass::getRawPtr(const Arguments& args)
{
	try {
		return Number::newNumber((intptr_t)blockEntity);
	}
	CATCH("Fail in getRawPtr!")
}

Local<Value> BlockEntityClass::getType()
{
	try {
		return Number::newNumber((int)Raw_GetBlockEntityType(blockEntity));
	}
	CATCH("Fail in getBlockType!")
}

Local<Value> BlockEntityClass::getNbt(const Arguments& args)
{
	try {
		return NbtCompound::newNBT(Tag::fromBlockEntity(blockEntity), true);
	}
	CATCH("Fail in getNbt!")
}

Local<Value> BlockEntityClass::setNbt(const Arguments& args)
{
	CHECK_ARGS_COUNT(args, 1);

	try {
		auto nbt = NbtCompound::extractNBT(args[0]);
		if (!nbt)
			return Local<Value>();    //Null

		nbt->setBlockEntity(blockEntity);
		return Boolean::newBoolean(true);
	}
	CATCH("Fail in setNbt!")
}