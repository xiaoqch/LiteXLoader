#include <list>
// Ignore error below
#include "ScriptX.h"
using namespace script;
#include "API/BaseAPI.h"
#include "API/BlockAPI.h"
#include "API/ItemAPI.h"
#include "API/EntityAPI.h"
#include "API/EventAPI.h"
#include "API/NbtAPI.h"
#include "API/GuiAPI.h"
#include "API/DbAPI.h"
#include "API/LoggerAPI.h"
#include "API/FileSystemAPI.h"
#include "API/NetworkAPI.h"
#include "API/PlayerAPI.h"
#include "API/ServerAPI.h"

void BindAPIs(std::shared_ptr<ScriptEngine> engine)
{
    
    //////////////// 全局函数 ////////////////

	engine->set("log", Function::newFunction(Log));
    engine->set("getLxlVersion",Function::newFunction(GetLxlVersion));

    engine->set("setTimeout",Function::newFunction(SetTimeout));
    engine->set("setInterval",Function::newFunction(SetInterval));
    engine->set("clearInterval",Function::newFunction(ClearInterval));


    //////////////// 基础类 ////////////////

    extern ClassDefine<IntPos> IntPosBuilder;
    extern ClassDefine<FloatPos> FloatPosBuilder;

    engine->registerNativeClass<IntPos>(IntPosBuilder);
    engine->registerNativeClass<FloatPos>(FloatPosBuilder);
    

    //////////////// 静态类 ////////////////

    extern ClassDefine<void> McClassBuilder;
    extern ClassDefine<void> SystemClassBuilder;
    extern ClassDefine<void> FileClassBuilder;
    extern ClassDefine<void> ServerClassBuilder;
    extern ClassDefine<void> LoggerClassBuilder;
    extern ClassDefine<void> ConfClassBuilder;
    extern ClassDefine<void> NetworkClassBuilder;
    
    engine->registerNativeClass(McClassBuilder);
    engine->registerNativeClass(SystemClassBuilder);
    engine->registerNativeClass(FileClassBuilder);
    engine->registerNativeClass(ServerClassBuilder);
    engine->registerNativeClass(LoggerClassBuilder);
    engine->registerNativeClass(ConfClassBuilder);
    engine->registerNativeClass(NetworkClassBuilder);


    //////////////// BlockAPI ////////////////

    extern ClassDefine<BlockClass> BlockClassBuilder;
    engine->registerNativeClass<BlockClass>(BlockClassBuilder);

    //////////////// DbAPI ////////////////

    extern ClassDefine<DbClass> DbClassBuilder;
    engine->registerNativeClass<DbClass>(DbClassBuilder);

    //////////////// EntityAPI ////////////////

    extern ClassDefine<EntityClass> EntityClassBuilder;
    engine->registerNativeClass<EntityClass>(EntityClassBuilder);

    //////////////// EventAPI ////////////////

    InitEventListeners();

    //////////////// GuiAPI ////////////////



    //////////////// ItemAPI ////////////////

    extern ClassDefine<ItemClass> ItemClassBuilder;
    engine->registerNativeClass<ItemClass>(ItemClassBuilder);

    //////////////// NbtAPI ////////////////



    //////////////// NetworkAPI ////////////////



    //////////////// PlayerAPI ////////////////

    extern ClassDefine<PlayerClass> PlayerClassBuilder;
    engine->registerNativeClass<PlayerClass>(PlayerClassBuilder);
}