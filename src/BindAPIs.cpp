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
#include "API/FileSystemAPI.h"
#include "API/NetworkAPI.h"
#include "API/PlayerAPI.h"
#include "API/StaticClasses.h"

void BindAPIs(std::shared_ptr<ScriptEngine> engine)
{
    //////////////// Base Classes ////////////////

    static ClassDefine<IntPos> IntPosBuilder =
        defineClass<IntPos>("IntPos")
            .constructor(&IntPos::create)
            .instanceProperty("x", &IntPos::getX, &IntPos::setX)
            .instanceProperty("y", &IntPos::getY, &IntPos::setY)
            .instanceProperty("z", &IntPos::getZ, &IntPos::setZ)
            .instanceProperty("dim", &IntPos::getDim, &IntPos::setDim)
            .build();

    static ClassDefine<FloatPos> FloatPosBuilder =
        defineClass<FloatPos>("FloatPos")
            .constructor(&FloatPos::create)
            .instanceProperty("x", &FloatPos::getX, &FloatPos::setX)
            .instanceProperty("y", &FloatPos::getY, &FloatPos::setY)
            .instanceProperty("z", &FloatPos::getZ, &FloatPos::setZ)
            .instanceProperty("dim", &FloatPos::getDim, &FloatPos::setDim)
            .build();
    
    static ClassDefine<void> McClassBuilder =
        defineClass("mc")
            .function("runcmd", &McClass::runcmd)
            .function("runcmdEx", &McClass::runcmdEx)
            .function("registerCmd", &McClass::registerCmd)
            .function("setServerMotd", &McClass::setServerMotd)
            .function("listen", &McClass::listen)
            .function("getPlayer", &McClass::getPlayer)
            .function("getOnlinePlayers", &McClass::getOnlinePlayers)
            .build();

    static ClassDefine<void> SystemClassBuilder =
        defineClass("system")
            .function("getTimeStr", &SystemClass::getTimeStr)
            .function("getTimeObj", &SystemClass::getTimeObj)
            .function("randomGuid", &SystemClass::randomGuid)
            .function("cmd", &SystemClass::cmd)
            .build();
    
    static ClassDefine<void> FileClassBuilder =
        defineClass("file")
            .function("read", &FileClass::read)
            .function("write", &FileClass::write)
            .function("writeLine", &FileClass::writeLine)

            .function("createDir", &FileClass::createDir)
            .function("copy", &FileClass::copy)
            .function("move", &FileClass::move)
            .function("rename", &FileClass::rename)
            .function("delete", &FileClass::del)
            .function("exists", &FileClass::exists)
            .build();
    
    engine->registerNativeClass<IntPos>(IntPosBuilder);
    engine->registerNativeClass<FloatPos>(FloatPosBuilder);
    engine->registerNativeClass(McClassBuilder);
    engine->registerNativeClass(SystemClassBuilder);
    engine->registerNativeClass(FileClassBuilder);


    //////////////// Global Functions ////////////////
	engine->set("log", Function::newFunction(Log));
    engine->set("getLxlVersion",Function::newFunction(GetLxlVersion));

    engine->set("setTimeout",Function::newFunction(SetTimeout));
    engine->set("setInterval",Function::newFunction(SetInterval));
    engine->set("clearInterval",Function::newFunction(ClearInterval));


    //////////////// BlockAPI ////////////////

    static ClassDefine<BlockClass> BlockClassBuilder =
        defineClass<BlockClass>("Block")
            .constructor(nullptr)
            .instanceProperty("name", &BlockClass::getName)
            .build();
    engine->registerNativeClass<BlockClass>(BlockClassBuilder);

    //////////////// DbAPI ////////////////



    //////////////// EntityAPI ////////////////

    static ClassDefine<EntityClass> EntityClassBuilder =
        defineClass<EntityClass>("Entity")
            .constructor(nullptr)
            .instanceProperty("name", &EntityClass::getName)
            .instanceProperty("pos", &EntityClass::getPos)

            .instanceFunction("teleport", &EntityClass::teleport)
            .instanceFunction("kill", &EntityClass::kill)
            .build();
    engine->registerNativeClass<EntityClass>(EntityClassBuilder);

    //////////////// EventAPI ////////////////

    InitEventListeners();

    //////////////// GuiAPI ////////////////



    //////////////// ItemAPI ////////////////

    static ClassDefine<ItemClass> ItemClassBuilder =
        defineClass<ItemClass>("Item")
            .constructor(nullptr)
            .instanceProperty("name", &ItemClass::getName)
            .instanceProperty("customName", &ItemClass::getCustomName)
            .instanceProperty("count", &ItemClass::getCount)

            .instanceFunction("setLore", &ItemClass::setLore)
            .build();
    engine->registerNativeClass<ItemClass>(ItemClassBuilder);

    //////////////// NbtAPI ////////////////



    //////////////// NetworkAPI ////////////////



    //////////////// PlayerAPI ////////////////

    static ClassDefine<PlayerClass> PlayerClassBuilder =
        defineClass<PlayerClass>("Player")
            .constructor(nullptr)
            .instanceProperty("name", &PlayerClass::getName)
            .instanceProperty("pos", &PlayerClass::getPos)
            .instanceProperty("realName", &PlayerClass::getRealName)
            .instanceProperty("xuid", &PlayerClass::getXuid)
            .instanceProperty("ip", &PlayerClass::getIP)

            .instanceFunction("isOP", &PlayerClass::isOP)
            .instanceFunction("getPlayerPermLevel", &PlayerClass::getPlayerPermLevel)
            .instanceFunction("setPlayerPermLevel", &PlayerClass::setPlayerPermLevel)

            .instanceFunction("runcmdAs", &PlayerClass::runcmdAs)
            .instanceFunction("teleport", &PlayerClass::teleport)
            .instanceFunction("kill", &PlayerClass::kill)
            .instanceFunction("kick", &PlayerClass::kick)
            .instanceFunction("tell", &PlayerClass::tell)
            .instanceFunction("getHand", &PlayerClass::getHand)
            .instanceFunction("rename", &PlayerClass::rename)
            .build();
    engine->registerNativeClass<PlayerClass>(PlayerClassBuilder);
}