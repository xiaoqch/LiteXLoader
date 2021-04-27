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
    engine->set("runCmd",Function::newFunction(RunCmd));
	engine->set("log", Function::newFunction(Log));

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
    auto funcListen = Function::newFunction(Listen);
    engine->set("listen", funcListen);

    //////////////// FileSystemAPI ////////////////



    //////////////// GuiAPI ////////////////



    //////////////// ItemAPI ////////////////

    static ClassDefine<ItemPointer> ItemPointerBuilder =
        defineClass<ItemPointer>("ItemPointer")
            .constructor(nullptr)
            .build();
    engine->registerNativeClass<ItemPointer>(ItemPointerBuilder);

    //////////////// NbtAPI ////////////////



    //////////////// NetworkAPI ////////////////



    //////////////// PlayerAPI ////////////////

    static ClassDefine<PlayerPointer> PlayerPointerBuilder =
        defineClass<PlayerPointer>("PlayerPointer")
            .constructor(nullptr)
            .build();
    engine->registerNativeClass<PlayerPointer>(PlayerPointerBuilder);

}