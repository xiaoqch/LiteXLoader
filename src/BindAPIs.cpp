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

void BindAPIs(std::shared_ptr<ScriptEngine> engine)
{
    //////////////// BaseAPI ////////////////

    static ClassDefine<IntPos> IntPosBuilder =
        defineClass<IntPos>("IntPos")
            .constructor()
            .constructor(&IntPos::create)
            .instanceProperty("x", &IntPos::getX, &IntPos::setX)
            .instanceProperty("y", &IntPos::getY, &IntPos::setY)
            .instanceProperty("z", &IntPos::getZ, &IntPos::setZ)
            .instanceProperty("dim", &IntPos::getDim, &IntPos::setDim)
            .build();

    static ClassDefine<FloatPos> FloatPosBuilder =
        defineClass<FloatPos>("FloatPos")
            .constructor()
            .constructor(&FloatPos::create)
            .instanceProperty("x", &FloatPos::getX, &FloatPos::setX)
            .instanceProperty("y", &FloatPos::getY, &FloatPos::setY)
            .instanceProperty("z", &FloatPos::getZ, &FloatPos::setZ)
            .instanceProperty("dim", &FloatPos::getDim, &FloatPos::setDim)
            .build();
    
    engine->registerNativeClass<IntPos>(IntPosBuilder);
    engine->registerNativeClass<FloatPos>(FloatPosBuilder);

    engine->set("getName",Function::newFunction(GetName));
    engine->set("getPos",Function::newFunction(GetPos));
    engine->set("teleport",Function::newFunction(Teleport));
    engine->set("kill",Function::newFunction(Kill));

    engine->set("runCmd",Function::newFunction(RunCmd));
    engine->set("runCmdEx",Function::newFunction(RunCmdEx));
    engine->set("registerCmd",Function::newFunction(RegisterCmd));

	engine->set("log", Function::newFunction(Log));
    engine->set("getTimeStr",Function::newFunction(GetTimeStr));
    engine->set("getTimeNow",Function::newFunction(GetTimeNow));
    engine->set("getLxlVersion",Function::newFunction(GetLxlVersion));

    engine->set("setTimeout",Function::newFunction(SetTimeout));
    engine->set("setInterval",Function::newFunction(SetInterval));
    engine->set("clearInterval",Function::newFunction(ClearInterval));

    //////////////// BlockAPI ////////////////

    static ClassDefine<BlockPointer> BlockPointerBuilder=
        defineClass<BlockPointer>("BlockPointer")
            .constructor(nullptr)
            .build();
    engine->registerNativeClass<BlockPointer>(BlockPointerBuilder);

    //////////////// DbAPI ////////////////



    //////////////// EntityAPI ////////////////

    static ClassDefine<EntityPointer> EntityPointerBuilder =
        defineClass<EntityPointer>("EntityPointer")
            .constructor(nullptr)
            .build();
    engine->registerNativeClass<EntityPointer>(EntityPointerBuilder);

    //////////////// EventAPI ////////////////

    RegisterEventListeners();
    engine->set("listen", Function::newFunction(Listen));

    //////////////// FileSystemAPI ////////////////

    engine->set("dirCreate",Function::newFunction(DirCreate));
    engine->set("pathCopy",Function::newFunction(PathCopy));
    engine->set("pathMove",Function::newFunction(PathMove));
    engine->set("pathRename",Function::newFunction(PathRename));
    engine->set("pathDelete",Function::newFunction(PathDelete));
    engine->set("pathExists",Function::newFunction(PathExists));

    engine->set("fileReadAll",Function::newFunction(FileReadAll));
    engine->set("fileWriteAll",Function::newFunction(FileWriteAll));
    engine->set("fileWriteLine",Function::newFunction(FileWriteLine));

    //////////////// GuiAPI ////////////////



    //////////////// ItemAPI ////////////////

    static ClassDefine<ItemPointer> ItemPointerBuilder =
        defineClass<ItemPointer>("ItemPointer")
            .constructor(nullptr)
            .build();
    engine->registerNativeClass<ItemPointer>(ItemPointerBuilder);

    engine->set("getCustomName",Function::newFunction(GetCustomName));
    engine->set("getCount",Function::newFunction(GetCount));
    engine->set("setLore",Function::newFunction(SetLore));

    //////////////// NbtAPI ////////////////



    //////////////// NetworkAPI ////////////////



    //////////////// PlayerAPI ////////////////

    static ClassDefine<PlayerPointer> PlayerPointerBuilder =
        defineClass<PlayerPointer>("PlayerPointer")
            .constructor(nullptr)
            .build();
    engine->registerNativeClass<PlayerPointer>(PlayerPointerBuilder);

    engine->set("getPlayer",Function::newFunction(GetPlayer));
    engine->set("getXuid",Function::newFunction(GetXuid));
    engine->set("getRealName",Function::newFunction(GetRealName));
    engine->set("getIP",Function::newFunction(GetIP));
    engine->set("getPlayerList",Function::newFunction(GetPlayerList));

    engine->set("isOP",Function::newFunction(IsOP));
    engine->set("getPlayerPermLevel",Function::newFunction(GetPlayerPermLevel));
    engine->set("setPlayerPermLevel",Function::newFunction(SetPlayerPermLevel));

    engine->set("kickPlayer",Function::newFunction(KickPlayer));
    engine->set("tell",Function::newFunction(Tell));
    engine->set("getHand",Function::newFunction(GetHand));
    engine->set("runCmdAs",Function::newFunction(RunCmdAs));
    engine->set("renamePlayer",Function::newFunction(RenamePlayer));
}