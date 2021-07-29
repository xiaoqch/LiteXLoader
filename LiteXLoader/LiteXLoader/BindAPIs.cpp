#include <ScriptX/ScriptX.h>
using namespace script;
#include <API/BaseAPI.h>
#include <API/BlockAPI.h>
#include <API/CommandAPI.h>
#include <API/DeviceAPI.h>
#include <API/ItemAPI.h>
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
#include <API/StaticClasses.h>

void BindAPIs(ScriptEngine *engine)
{
    
    //////////////// 全局函数 ////////////////

	engine->set("log", Function::newFunction(Log));

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
    engine->registerNativeClass(FileClassBuilder);
    engine->registerNativeClass(LoggerClassBuilder);
    engine->registerNativeClass(DataClassBuilder);
    engine->registerNativeClass(MoneyClassBuilder);
    engine->registerNativeClass(NetworkClassBuilder);
    engine->registerNativeClass(LxlClassBuilder);
    engine->registerNativeClass(NbtStaticBuilder);


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

    extern ClassDefine<EntityClass> EntityClassBuilder;
    engine->registerNativeClass<EntityClass>(EntityClassBuilder);

    extern ClassDefine<SimpleFormClass> SimpleFormClassBuilder;
    engine->registerNativeClass<SimpleFormClass>(SimpleFormClassBuilder);

    extern ClassDefine<CustomFormClass> CustomFormClassBuilder;
    engine->registerNativeClass<CustomFormClass>(CustomFormClassBuilder);

    extern ClassDefine<ItemClass> ItemClassBuilder;
    engine->registerNativeClass<ItemClass>(ItemClassBuilder);

    extern ClassDefine<PlayerClass> PlayerClassBuilder;
    engine->registerNativeClass<PlayerClass>(PlayerClassBuilder);

    extern ClassDefine<NbtValue> NbtValueBuilder;
    engine->registerNativeClass<NbtValue>(NbtValueBuilder);

    extern ClassDefine<NbtList> NbtListBuilder;
    engine->registerNativeClass<NbtList>(NbtListBuilder);

    extern ClassDefine<NbtCompound> NbtCompoundBuilder;
    engine->registerNativeClass<NbtCompound>(NbtCompoundBuilder);
}