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
    //////////////// 初始化 ////////////////

    InitEventListeners();
    
    
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

    extern ClassDefine<void> McClassBuilder;
    engine->registerNativeClass(McClassBuilder);

    extern ClassDefine<void> SystemClassBuilder;
    engine->registerNativeClass(SystemClassBuilder);

    extern ClassDefine<void> FileClassBuilder;
    engine->registerNativeClass(FileClassBuilder);

    extern ClassDefine<void> LoggerClassBuilder;
    engine->registerNativeClass(LoggerClassBuilder);

    extern ClassDefine<void> ConfClassBuilder;
    engine->registerNativeClass(ConfClassBuilder);

    extern ClassDefine<void> NetworkClassBuilder;
    engine->registerNativeClass(NetworkClassBuilder);

    extern ClassDefine<void> LxlClassBuilder;
    engine->registerNativeClass(LxlClassBuilder);


    //////////////// 实例类 ////////////////

    extern ClassDefine<BlockClass> BlockClassBuilder;
    engine->registerNativeClass<BlockClass>(BlockClassBuilder);

    extern ClassDefine<DbClass> DbClassBuilder;
    engine->registerNativeClass<DbClass>(DbClassBuilder);

    extern ClassDefine<EntityClass> EntityClassBuilder;
    engine->registerNativeClass<EntityClass>(EntityClassBuilder);

    extern ClassDefine<FormClass> FormClassBuilder;
    engine->registerNativeClass<FormClass>(FormClassBuilder);

    extern ClassDefine<ItemClass> ItemClassBuilder;
    engine->registerNativeClass<ItemClass>(ItemClassBuilder);

    extern ClassDefine<PlayerClass> PlayerClassBuilder;
    engine->registerNativeClass<PlayerClass>(PlayerClassBuilder);
}