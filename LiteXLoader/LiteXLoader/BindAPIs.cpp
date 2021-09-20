#include <ScriptX/ScriptX.h>
using namespace script;
#include <API/BaseAPI.h>
#include <API/BlockAPI.h>
#include <API/BlockEntityAPI.h>
#include <API/CommandAPI.h>
#include <API/DeviceAPI.h>
#include <API/ItemAPI.h>
#include <API/ContainerAPI.h>
#include <API/EntityAPI.h>
#include <API/NbtAPI.h>
#include <API/GuiAPI.h>
#include <API/DataAPI.h>
#include <API/LoggerAPI.h>
#include <API/FileSystemAPI.h>
#include <API/NetworkAPI.h>
#include <API/PlayerAPI.h>
#include <API/ServerAPI.h>
#include <API/ScriptAPI.h>
#include <API/ScoreboardAPI.h>
#include <API/StaticClasses.h>
#include <API/PacketAPI.h>
#include <API/GameUtilsAPI.h>

void BindAPIs(ScriptEngine *engine)
{
    
    //////////////// 全局函数 ////////////////

	engine->set("log", Function::newFunction(Log));
	engine->set("colorLog", Function::newFunction(ColorLog));
    engine->set("fastLog", Function::newFunction(FastLog));

    engine->set("setTimeout",Function::newFunction(SetTimeout));
    engine->set("setInterval",Function::newFunction(SetInterval));
    engine->set("clearInterval",Function::newFunction(ClearInterval));


    //////////////// 基础类 ////////////////

    extern ClassDefine<IntPos> IntPosBuilder;
    engine->registerNativeClass<IntPos>(IntPosBuilder);

    extern ClassDefine<FloatPos> FloatPosBuilder;
    engine->registerNativeClass<FloatPos>(FloatPosBuilder);
    

    //////////////// 静态类 ////////////////

    engine->registerNativeClass(McClassBuilder);
    engine->registerNativeClass(SystemClassBuilder);
    engine->registerNativeClass(LoggerClassBuilder);
    engine->registerNativeClass(DataClassBuilder);
    engine->registerNativeClass(MoneyClassBuilder);
    engine->registerNativeClass(NetworkClassBuilder);
    engine->registerNativeClass(LxlClassBuilder);
    engine->registerNativeClass(NbtStaticBuilder);
    engine->registerNativeClass(TextClassBuilder);


    //////////////// 实例类 ////////////////

    extern ClassDefine<BlockClass> BlockClassBuilder;
    engine->registerNativeClass<BlockClass>(BlockClassBuilder);

    extern ClassDefine<DbClass> DbClassBuilder;
    engine->registerNativeClass<DbClass>(DbClassBuilder);

    extern ClassDefine<ConfJsonClass> ConfJsonClassBuilder;
    engine->registerNativeClass<ConfJsonClass>(ConfJsonClassBuilder);

    extern ClassDefine<ConfIniClass> ConfIniClassBuilder;
    engine->registerNativeClass<ConfIniClass>(ConfIniClassBuilder);

    extern ClassDefine<DeviceClass> DeviceClassBuilder;
    engine->registerNativeClass<DeviceClass>(DeviceClassBuilder);

    extern ClassDefine<ContainerClass> ContainerClassBuilder;
    engine->registerNativeClass<ContainerClass>(ContainerClassBuilder);

    extern ClassDefine<EntityClass> EntityClassBuilder;
    engine->registerNativeClass<EntityClass>(EntityClassBuilder);

    extern ClassDefine<FileClass> FileClassBuilder;
    engine->registerNativeClass<FileClass>(FileClassBuilder);

    extern ClassDefine<WSClientClass> WSClientClassBuilder;
    engine->registerNativeClass<WSClientClass>(WSClientClassBuilder);

    extern ClassDefine<BlockEntityClass> BlockEntityClassBuilder;
    engine->registerNativeClass<BlockEntityClass>(BlockEntityClassBuilder);

    extern ClassDefine<SimpleFormClass> SimpleFormClassBuilder;
    engine->registerNativeClass<SimpleFormClass>(SimpleFormClassBuilder);

    extern ClassDefine<CustomFormClass> CustomFormClassBuilder;
    engine->registerNativeClass<CustomFormClass>(CustomFormClassBuilder);

    extern ClassDefine<ItemClass> ItemClassBuilder;
    engine->registerNativeClass<ItemClass>(ItemClassBuilder);

    extern ClassDefine<PlayerClass> PlayerClassBuilder;
    engine->registerNativeClass<PlayerClass>(PlayerClassBuilder);

    extern ClassDefine<ObjectiveClass> ObjectiveClassBuilder;
    engine->registerNativeClass<ObjectiveClass>(ObjectiveClassBuilder);

    extern ClassDefine<PacketClass> PacketClassBuilder;
    engine->registerNativeClass<PacketClass>(PacketClassBuilder);

    extern ClassDefine<NbtEndClass> NbtEndClassBuilder;
    engine->registerNativeClass<NbtEndClass>(NbtEndClassBuilder);

    extern ClassDefine<NbtByteClass> NbtByteClassBuilder;
    engine->registerNativeClass<NbtByteClass>(NbtByteClassBuilder);

    extern ClassDefine<NbtShortClass> NbtShortClassBuilder;
    engine->registerNativeClass<NbtShortClass>(NbtShortClassBuilder);

    extern ClassDefine<NbtIntClass> NbtIntClassBuilder;
    engine->registerNativeClass<NbtIntClass>(NbtIntClassBuilder);

    extern ClassDefine<NbtLongClass> NbtLongClassBuilder;
    engine->registerNativeClass<NbtLongClass>(NbtLongClassBuilder);

    extern ClassDefine<NbtFloatClass> NbtFloatClassBuilder;
    engine->registerNativeClass<NbtFloatClass>(NbtFloatClassBuilder);

    extern ClassDefine<NbtDoubleClass> NbtDoubleClassBuilder;
    engine->registerNativeClass<NbtDoubleClass>(NbtDoubleClassBuilder);

    extern ClassDefine<NbtStringClass> NbtStringClassBuilder;
    engine->registerNativeClass<NbtStringClass>(NbtStringClassBuilder);

    extern ClassDefine<NbtByteArrayClass> NbtByteArrayClassBuilder;
    engine->registerNativeClass<NbtByteArrayClass>(NbtByteArrayClassBuilder);

    extern ClassDefine<NbtListClass> NbtListClassBuilder;
    engine->registerNativeClass<NbtListClass>(NbtListClassBuilder);

    extern ClassDefine<NbtCompoundClass> NbtCompoundClassBuilder;
    engine->registerNativeClass<NbtCompoundClass>(NbtCompoundClassBuilder);

}