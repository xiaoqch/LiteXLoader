#include "EventAPI.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <exception>
#include <cstdarg>
#include <Kernel/Global.h>
#include <Kernel/Base.h>
#include <Kernel/Block.h>
#include <Kernel/Item.h>
#include <Kernel/Entity.h>
#include <Kernel/Player.h>
#include <Kernel/SymbolHelper.h>
#include <Kernel/Packet.h>
#include <Kernel/Global.h>
#include <Engine/EngineOwnData.h>
#include <Engine/GlobalShareData.h>
#include <Engine/LocalShareData.h>
#include "APIHelp.h"
#include "BaseAPI.h"
#include "BlockAPI.h"
#include "CommandAPI.h"
#include "ItemAPI.h"
#include "EntityAPI.h"
#include "PlayerAPI.h"
#include <Loader.h>
#include <Configs.h>
#include <CheckUpdate.h>
using namespace std;
using namespace script;

//////////////////// Listeners ////////////////////

enum class EVENT_TYPES : int
{
    onPreJoin=0, onJoin, onLeft, onPlayerCmd, onChat, onPlayerDie, 
    onRespawn, onChangeDim, onJump, onSneak, onAttack, onEat, onMove, onSetArmor, onRide,
    onUseItem, onTakeItem, onDropItem, onUseItemOn,
    onRespawn, onChangeDim, onJump, onSneak, onAttack, onEat, onMove, onSetArmor, onPlayerRide,
    onUseItem, onTakeItem, onDropItem, onUseItemOn, onInventoryChange,
    onDestroyingBlock, onDestroyBlock, onWitherBossDestroy, onPlaceBlock,
    onOpenContainer, onCloseContainer, onContainerChange, onOpenContainerScreen,
    onMobDie, onMobHurt, onExplode, onBlockExploded, onCmdBlockExecute,
    onProjectileHit, onBlockInteracted, onUseRespawnAnchor, onFarmLandDecay, onUseFrameBlock,
    onPistonPush, onHopperSearchItem, onHopperPushOut, onFireSpread, onFishingHookRetrieve,
    onScoreChanged, onServerStarted, onConsoleCmd, onFormSelected, onConsoleOutput,
    EVENT_COUNT
};
static const std::unordered_map<string, EVENT_TYPES> EventsMap{
    {"onPreJoin",EVENT_TYPES::onPreJoin},
    {"onJoin",EVENT_TYPES::onJoin},
    {"onLeft",EVENT_TYPES::onLeft},
    {"onPlayerCmd",EVENT_TYPES::onPlayerCmd},
    {"onChat",EVENT_TYPES::onChat},
    {"onPlayerDie",EVENT_TYPES::onPlayerDie},
    {"onRespawn",EVENT_TYPES::onRespawn},
    {"onChangeDim",EVENT_TYPES::onChangeDim},
    {"onJump",EVENT_TYPES::onJump},
    {"onSneak",EVENT_TYPES::onSneak},
    {"onAttack",EVENT_TYPES::onAttack},
    {"onEat",EVENT_TYPES::onEat},
    {"onMove",EVENT_TYPES::onMove},
    {"onSetArmor",EVENT_TYPES::onSetArmor},
    {"onRide",EVENT_TYPES::onRide},
    {"onMobDie",EVENT_TYPES::onMobDie},
    {"onMobHurt",EVENT_TYPES::onMobHurt},
    {"onUseItem",EVENT_TYPES::onUseItem},
    {"onTakeItem",EVENT_TYPES::onTakeItem},
    {"onDropItem",EVENT_TYPES::onDropItem},
    {"onUseItemOn",EVENT_TYPES::onUseItemOn},
    {"onInventoryChange",EVENT_TYPES::onInventoryChange},
    {"onDestroyingBlock",EVENT_TYPES::onDestroyingBlock},
    {"onDestroyBlock",EVENT_TYPES::onDestroyBlock},
    {"onWitherBossDestroy",EVENT_TYPES::onWitherBossDestroy},
    {"onPlaceBlock",EVENT_TYPES::onPlaceBlock},
    {"onExplode",EVENT_TYPES::onExplode},
    {"onBlockExploded",EVENT_TYPES::onBlockExploded},
    {"onOpenContainer",EVENT_TYPES::onOpenContainer},
    {"onCloseContainer",EVENT_TYPES::onCloseContainer},
    {"onContainerChangeSlot",EVENT_TYPES::onContainerChange},
    {"onContainerChange",EVENT_TYPES::onContainerChange},
    {"onOpenContainerScreen",EVENT_TYPES::onOpenContainerScreen},
    {"onCmdBlockExecute",EVENT_TYPES::onCmdBlockExecute},
    {"onProjectileHit",EVENT_TYPES::onProjectileHit},
    {"onBlockInteracted",EVENT_TYPES::onBlockInteracted},
    {"onUseRespawnAnchor",EVENT_TYPES::onUseRespawnAnchor},
    {"onFarmLandDecay",EVENT_TYPES::onFarmLandDecay},
    {"onUseFrameBlock",EVENT_TYPES::onUseFrameBlock},
    {"onPistonPush",EVENT_TYPES::onPistonPush},
    {"onHopperSearchItem",EVENT_TYPES::onHopperSearchItem},
    {"onHopperPushOut",EVENT_TYPES::onHopperPushOut},
    {"onFireSpread",EVENT_TYPES::onFireSpread},
    {"onFishingHookRetrieve",EVENT_TYPES::onFishingHookRetrieve},
    {"onScoreChanged",EVENT_TYPES::onScoreChanged},
    {"onServerStarted",EVENT_TYPES::onServerStarted},
    {"onConsoleCmd",EVENT_TYPES::onConsoleCmd},
    {"onConsoleOutput",EVENT_TYPES::onConsoleOutput},
    {"onFormSelected",EVENT_TYPES::onFormSelected}
};
struct ListenerListType
{
    ScriptEngine *engine;
    Global<Function> func;
};
//监听器表
static std::vector<ListenerListType> listenerList[int(EVENT_TYPES::EVENT_COUNT)];

//调用事件监听函数
#define CallEvent(TYPE,...) \
    std::vector<ListenerListType> &nowList = listenerList[int(TYPE)]; \
    for(int i = 0; i < nowList.size(); ++i) { \
        EngineScope enter(nowList[i].engine); \
        try{ \
            nowList[i].func.get().call({},__VA_ARGS__); \
        } \
        catch(const Exception& e) \
        { \
            ERROR("Event Callback Failed!"); \
            ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName); \
            ERRPRINT(e); \
        } \
    }

//调用事件监听函数，拦截返回false
#define CallEventEx(TYPE,...) \
    std::vector<ListenerListType> &nowList = listenerList[int(TYPE)]; \
    bool passToBDS = true; \
    for(int i = 0; i < nowList.size(); ++i) { \
        EngineScope enter(nowList[i].engine); \
        try{ \
            auto result = nowList[i].func.get().call({},__VA_ARGS__); \
            if(result.isBoolean() && result.asBoolean().value() == false) \
                passToBDS = false; \
        } \
        catch(const Exception& e) \
        { \
            ERROR("Event Callback Failed!"); \
            ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName); \
            ERRPRINT(e); \
        } \
    }\
    if(!passToBDS) { return false; }

//调用事件监听函数，拦截返回RETURN_VALUE
#define CallEventRtn(TYPE,RETURN_VALUE,...) \
    std::vector<ListenerListType> &nowList = listenerList[int(TYPE)]; \
    bool passToBDS = true; \
    for(int i = 0; i < nowList.size(); ++i) { \
        EngineScope enter(nowList[i].engine); \
        try{ \
            auto result = nowList[i].func.get().call({},__VA_ARGS__); \
            if(result.isBoolean() && result.asBoolean().value() == false) \
                passToBDS = false; \
        } \
        catch(const Exception& e) \
        { \
            ERROR("Event Callback Failed!"); \
            ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName); \
            ERRPRINT(e); \
        } \
    }\
    if(!passToBDS) { return RETURN_VALUE; }


#define IF_LISTENED(EVENT) if(!listenerList[int(EVENT)].empty()) { try
#define IF_LISTENDED_END() catch(const seh_exception &e){ ERROR("SEH Exception Caught!"); ERRPRINT(e.what()); } }


//////////////////// APIs ////////////////////

Local<Value> Listen(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2);
    CHECK_ARG_TYPE(args[0], ValueKind::kString);
    CHECK_ARG_TYPE(args[1], ValueKind::kFunction);

    try{
        return Boolean::newBoolean(LxlAddEventListener(EngineScope::currentEngine(),args[0].toStr(),args[1].asFunction()));
    }
    CATCH("Fail to Bind Listener!")
}


//////////////////// Funcs ////////////////////

bool LxlAddEventListener(ScriptEngine *engine, const string &eventName, const Local<Function> &func)
{
    try {
        int eventId = int(EventsMap.at(eventName));
        listenerList[eventId].push_back({ engine,Global<Function>(func) });
        return true;
    }
    catch (const std::logic_error& e)
    {
        ERROR("Event \"" + eventName + "\" No Found!\n");
        return false;
    }
}

bool LxlRemoveAllEventListeners(ScriptEngine* engine)
{
    for (auto& listeners : listenerList)
    {
        for (int i = 0; i < listeners.size(); ++i)
        {
            if (listeners[i].engine == engine)
            {
                listeners.erase(listeners.begin() + i);
                --i;
            }
        }
    }
    return true;
}

bool LxlRecallOnServerStarted(ScriptEngine* engine)
{
    std::vector<ListenerListType>& nowList = listenerList[int(EVENT_TYPES::onServerStarted)];
    for (int i = 0; i < nowList.size(); ++i)
    {
        if (nowList[i].engine == engine)
        {
            EngineScope enter(nowList[i].engine);
            try {
                nowList[i].func.get().call();
            }
            catch (const Exception& e)
            {
                ERROR("Event Callback Failed!");
                ERRPRINT("[Error] In Plugin: " + ENGINE_OWN_DATA()->pluginName);
                ERRPRINT(e);
                return false;
            }
            break;
        }
    }
    return true;
}

//////////////////// Hook ////////////////////

void InitEventListeners()
{
// ===== onPreJoin =====
    Event::addEventListener([](JoinEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onPreJoin)
        {
            CallEvent(EVENT_TYPES::onPreJoin, PlayerClass::newPlayer(ev.Player));
        }
        IF_LISTENDED_END();
    });

// ===== onLeft =====
    Event::addEventListener([](LeftEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onLeft)
        {
            CallEvent(EVENT_TYPES::onLeft, PlayerClass::newPlayer(ev.Player));
        }
        IF_LISTENDED_END();
    });

// ===== onChat =====
    Event::addEventListener([](ChatEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onChat)
        {
            CallEventEx(EVENT_TYPES::onChat, PlayerClass::newPlayer(ev.pl), ev.msg);
        }
        IF_LISTENDED_END();
        return true;
    });

// ===== onPlayerDie =====
    Event::addEventListener([](PlayerDeathEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onPlayerDie)
        {
            CallEvent(EVENT_TYPES::onPlayerDie, PlayerClass::newPlayer(ev.Player));
        }
        IF_LISTENDED_END();
    });

// ===== onChangeDimension =====
    Event::addEventListener([](ChangeDimEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onChangeDim)
        {
            CallEvent(EVENT_TYPES::onChangeDim, PlayerClass::newPlayer(ev.Player));
        }
        IF_LISTENDED_END();
    });

// ===== onCmdBlockExecute =====
    Event::addEventListener([](CmdBlockExeEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onCmdBlockExecute)
        {
            CallEventEx(EVENT_TYPES::onCmdBlockExecute, ev.cmd, IntPos::newPos(ev.bpos.x, ev.bpos.y, ev.bpos.z));
        }
        IF_LISTENDED_END();
        return true;
    });

// ===== onMobDie =====
    Event::addEventListener([](MobDieEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onMobDie)
        {
            if (ev.mob && ev.DamageSource)
            {
                CallEvent(EVENT_TYPES::onMobDie, EntityClass::newEntity(ev.mob), EntityClass::newEntity(ev.DamageSource));
            }
        }
        IF_LISTENDED_END();
    });

// For RegisterCmd...
    Event::addEventListener([](RegCmdEV ev)
    {
        CmdReg = ev.CMDRg;
        isCmdRegisterEnabled = true;

        //处理延迟注册
        ProcessRegCmdQueue();
    });


// ===== onServerStarted =====
    Event::addEventListener([](ServerStartedEV ev)
    {
        //标记已启动
        if (!isServerStarted)
        {
            isServerStarted = true;

            //注册预置命令
            RegisterBuiltinCmds();

            //更新检查
            if (localShareData->isFirstInstance)
            {
                CheckUpdate();
            }

            IF_LISTENED(EVENT_TYPES::onServerStarted)
            {
                CallEvent(EVENT_TYPES::onServerStarted);
            }
            IF_LISTENDED_END();
        }
    });
}

// 植入tick
THook(void, "?tick@ServerLevel@@UEAAXXZ",
    void* _this)
{
    try
    {
        for (auto engine : lxlModules)
        {
            EngineScope enter(engine);
            engine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
        }
    }
    catch (...)
    {
        ;
    }
    return original(_this);
}

// For device information
class ConnectionRequest;
THook(void, "?sendLoginMessageLocal@ServerNetworkHandler@@QEAAXAEBVNetworkIdentifier@@"
    "AEBVConnectionRequest@@AEAVServerPlayer@@@Z",
    ServerNetworkHandler* _this, NetworkIdentifier* ni, ConnectionRequest* cr, ServerPlayer* sp)
{
    string id = "", os = "";
    SymCall("?getDeviceId@ConnectionRequest@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
        void, ConnectionRequest*, string*)(cr, &id);
    int type = SymCall("?getDeviceOS@ConnectionRequest@@QEBA?AW4BuildPlatform@@XZ",
        int, ConnectionRequest*, string*)(cr, &os);

    localShareData->deviceInfoRecord[(uintptr_t)sp] = { id,type };
    return original(_this, ni, cr, sp);
}


// ===== onJoin =====
THook(bool, "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    ServerPlayer* _this)
{
    IF_LISTENED(EVENT_TYPES::onJoin)
    {
        CallEvent(EVENT_TYPES::onJoin, PlayerClass::newPlayer(_this));
    }
    IF_LISTENDED_END();
    return original(_this);
}

// ===== onAttack =====
THook(bool, "?attack@Player@@UEAA_NAEAVActor@@AEBW4ActorDamageCause@@@Z",
    Player* _this, Actor* ac, int *damageCause)
{
    IF_LISTENED(EVENT_TYPES::onAttack)
    {
        if (ac)
        {
            CallEventEx(EVENT_TYPES::onAttack, PlayerClass::newPlayer(_this), EntityClass::newEntity(ac));
        }
    }
    IF_LISTENDED_END();
    return original(_this, ac, damageCause);
}

// ===== onEat =====
THook(void, "?eat@Player@@QEAAXAEBVItemStack@@@Z",
    Player* _this, ItemStack* eaten)
{
    IF_LISTENED(EVENT_TYPES::onEat)
    {
        CallEvent(EVENT_TYPES::onEat, PlayerClass::newPlayer(_this), ItemClass::newItem(eaten));
    }
    IF_LISTENDED_END();
    return original(_this, eaten);
}

// ===== onMove =====
THook(void, "?sendPlayerMove@PlayerEventCoordinator@@QEAAXAEAVPlayer@@@Z",
    void *_this, Player* pl)
{
    IF_LISTENED(EVENT_TYPES::onMove)
    {
        CallEvent(EVENT_TYPES::onMove, PlayerClass::newPlayer(pl), FloatPos::newPos(Raw_GetPlayerPos(pl)));
    }
    IF_LISTENDED_END();
    return original(_this, pl);
}

// ===== onShootFireworkWithCrossbow =====
/*
THook(void, "?_shootFirework@CrossbowItem@@AEBAXAEBVItemInstance@@AEAVPlayer@@@Z",
    void* _this, void* a2, Player* a3)
{
    IF_LISTENED(EVENT_TYPES::onShootFireworkWithCrossbow)
    {
        CallEvent(EVENT_TYPES::onShootFireworkWithCrossbow, PlayerClass::newPlayer(a3));
    }
    IF_LISTENDED_END();
    // 不original即可拦截，等待CallEvent重写...
    // Event -> [1]用弩发射烟花的玩家对象

    original(_this, a3, a3);
}
*/

// ===== onSetArmor =====
THook(void, "?setArmor@Player@@UEAAXW4ArmorSlot@@AEBVItemStack@@@Z",
    Player* _this, unsigned slot, ItemStack* it)
{
    IF_LISTENED(EVENT_TYPES::onSetArmor)
    {
        CallEvent(EVENT_TYPES::onSetArmor, PlayerClass::newPlayer(_this), Number::newNumber((int)slot), ItemClass::newItem(it));
    }
    IF_LISTENDED_END();
    return original(_this, slot, it);
}

// ===== onRide =====
THook(bool, "?canAddRider@Actor@@UEBA_NAEAV1@@Z",
    Actor* a1, Actor* a2)
{
    IF_LISTENED(EVENT_TYPES::onRide)
    {
        CallEventEx(EVENT_TYPES::onRide, EntityClass::newEntity(a2), EntityClass::newEntity(a1));
    }
        IF_LISTENDED_END();
    return original(a1, a2);
}

// ===== onStepOnPressurePlate =====
/*
THook(void, "?entityInside@BasePressurePlateBlock@@UEBAXAEAVBlockSource@@AEBVBlockPos@@AEAVActor@@@Z",
    void* _this, BlockSource* a2, BlockPos* a3, Actor* a4)
{
    IF_LISTENED(EVENT_TYPES::onStepOnPressurePlate)
    {
        Block* bl = Raw_GetBlockByPos(a3->x, a3->y, a3->z, a2);
        CallEvent(EVENT_TYPES::onStepOnPressurePlate, EntityClass::newEntity(a4), BlockClass::newBlock(bl, a3, a2));
    }
    IF_LISTENDED_END();
    // 不original即可拦截，等待CallEvent重写...
    // Event -> [1]踩压力板的实体对象 [2]压力板的方块对象

    original(_this, a2, a3, a4);
}
*/

// ===== onRespawn =====
THook(bool, "?respawn@Player@@UEAAXXZ",
    Player* pl)
{
    IF_LISTENED(EVENT_TYPES::onRespawn)
    {
        CallEventEx(EVENT_TYPES::onRespawn, PlayerClass::newPlayer(pl));
    }
    IF_LISTENDED_END();
    return original(pl);
}

// ===== onJump =====
THook(void, "?jumpFromGround@Player@@UEAAXXZ",
    Player* pl)
{
    IF_LISTENED(EVENT_TYPES::onJump)
    {
        CallEvent(EVENT_TYPES::onJump, PlayerClass::newPlayer(pl));
    }
    IF_LISTENDED_END();
    return original(pl);
}

// ===== onSneak =====
THook(void, "?sendActorSneakChanged@ActorEventCoordinator@@QEAAXAEAVActor@@_N@Z",
    void* _this, Actor* ac, bool isSneaking)
{
    IF_LISTENED(EVENT_TYPES::onSneak)
    {
        CallEvent(EVENT_TYPES::onSneak, PlayerClass::newPlayer((Player*)ac), Boolean::newBoolean(isSneaking));
    }
    IF_LISTENDED_END();
    return original(_this, ac, isSneaking);
}

// ===== onDropItem =====
THook(bool, "?drop@Player@@UEAA_NAEBVItemStack@@_N@Z",
    Player* _this, ItemStack* a2, bool a3)
{
    IF_LISTENED(EVENT_TYPES::onDropItem)
    {
        CallEventEx(EVENT_TYPES::onDropItem, PlayerClass::newPlayer(_this), ItemClass::newItem(a2));
    }
    IF_LISTENDED_END();
    return original(_this, a2, a3);
}

// ===== onTakeItem =====
THook(bool, "?take@Player@@QEAA_NAEAVActor@@HH@Z",
    Player* _this, Actor* actor, int a2, int a3)
{
    IF_LISTENED(EVENT_TYPES::onTakeItem)       //################### 有无办法改成获取item ###################
    {
        CallEventEx(EVENT_TYPES::onTakeItem, PlayerClass::newPlayer(_this), EntityClass::newEntity(actor));
    }
    IF_LISTENDED_END();
    return original(_this, actor, a2, a3);
}

// ===== onUseItem =====
THook(bool, "?baseUseItem@GameMode@@QEAA_NAEAVItemStack@@@Z", void* _this, ItemStack& item)
{
    IF_LISTENED(EVENT_TYPES::onUseItem)
    {
        auto sp = dAccess<ServerPlayer*, 8>(_this);

        CallEventEx(EVENT_TYPES::onUseItem, PlayerClass::newPlayer(sp), ItemClass::newItem(&item));
    }
    IF_LISTENDED_END();
    return original(_this, item);
}

// ===== onUseItemOn =====
THook(bool, "?useItemOn@GameMode@@UEAA_NAEAVItemStack@@AEBVBlockPos@@EAEBVVec3@@PEBVBlock@@@Z",
    void* _this, ItemStack* item, BlockPos* bp, unsigned char side, Vec3* a5, Block* bl)
{
    IF_LISTENED(EVENT_TYPES::onUseItemOn)
    {
        auto sp = dAccess<ServerPlayer*, 8>(_this);

        CallEventEx(EVENT_TYPES::onUseItemOn, PlayerClass::newPlayer(sp), ItemClass::newItem(item), BlockClass::newBlock(bl, bp, WPlayer(*sp).getDimID()));
    }
    IF_LISTENDED_END();
    return original(_this, item, bp, side, a5, bl);
}

// ===== onInventoryChange =====
THook(void, "?inventoryChanged@Player@@UEAAXAEAVContainer@@HAEBVItemStack@@1_N@Z",
    Player* _this, void* container, int slotNumber, ItemStack* oldItem, ItemStack* newItem, bool is)
{
    IF_LISTENED(EVENT_TYPES::onInventoryChange)
    {
        bool isPutIn = Raw_IsNull(oldItem);
        CallEvent(EVENT_TYPES::onInventoryChange, PlayerClass::newPlayer((Player*)_this), slotNumber,
            isPutIn, ItemClass::newItem(isPutIn ? newItem : oldItem));
    }
    IF_LISTENDED_END();

    return original(_this, container, slotNumber, oldItem, newItem, is);
}

// ===== onDestroyingBlock =====
THook(float, "?getDestroySpeed@Player@@QEBAMAEBVBlock@@@Z",
    Player* _this, Block* bl)
{
    IF_LISTENED(EVENT_TYPES::onDestroyingBlock)     //############## 找个可以获取坐标的？ ################
    {
        CallEvent(EVENT_TYPES::onDestroyingBlock, PlayerClass::newPlayer(_this), BlockClass::newBlock(bl));
    }
    IF_LISTENDED_END();
    return original(_this, bl);
}

// ===== onDestroyBlock =====
THook(bool, "?checkBlockDestroyPermissions@BlockSource@@QEAA_NAEAVActor@@AEBVBlockPos@@AEBVItemStackBase@@_N@Z",
    BlockSource * _this, Actor* pl, BlockPos* pos, ItemStack* a3, bool a4)
{
    IF_LISTENED(EVENT_TYPES::onDestroyBlock)
    {
        auto block = Raw_GetBlockByPos(pos->x, pos->y, pos->z, _this);

        CallEventEx(EVENT_TYPES::onDestroyBlock, PlayerClass::newPlayer((Player*)pl), BlockClass::newBlock(block,pos,_this));
    }
    IF_LISTENDED_END();
    return original(_this, pl, pos,a3, a4);
}

// ===== onWitherBossDestroy =====

THook(bool, "?canDestroy@WitherBoss@@SA_NAEBVBlock@@@Z",
    Block* a1)
{
    IF_LISTENED(EVENT_TYPES::onWitherBossDestroy)
    {
        CallEventEx(EVENT_TYPES::onWitherBossDestroy, BlockClass::newBlock(a1));
    }
    IF_LISTENDED_END();
    return original(a1);
}

THook(bool, "?canDestroyBlock@WitherSkull@@UEBA_NAEBVBlock@@@Z",
    void* _this, Block* a2)
{
    IF_LISTENED(EVENT_TYPES::onWitherBossDestroy)
    {
        CallEventEx(EVENT_TYPES::onWitherBossDestroy, BlockClass::newBlock(a2));
    }
    IF_LISTENDED_END();
    return original(_this, a2);
}

/*
THook(void, "?_destroyBlocks@WitherBoss@@AEAAXAEAVLevel@@AEBVAABB@@AEAVBlockSource@@H@Z",
    void* _this, struct Level* a2, const struct AABB* a3, struct BlockSource* a4, int a5)
{
    IF_LISTENED(EVENT_TYPES::onWitherBossDestroy)
    {
        auto ac = (Actor*)_this;
        auto dimid = dAccess<int, -96>(a4);
        Vec3 posA = a3->p1;
        Vec3 posB = a3->p2;
        CallEvent(EVENT_TYPES::onWitherBossDestroy, EntityClass::newEntity(ac), IntPos::newPos(posA.x, posA.y, posA.z, dimid), IntPos::newPos(posB.x, posB.y, posB.z, dimid));
    }
    IF_LISTENDED_END();
    // 不original即可拦截，等待CallEvent重写...
    // Event -> [1]凋灵实体对象 [2]AABB(AA) [3]AABB(BB)

    original(_this, a2, a3, a4, a5);
}
*/

// ===== onPlaceBlock =====
THook(bool, "?mayPlace@BlockSource@@QEAA_NAEBVBlock@@AEBVBlockPos@@EPEAVActor@@_N@Z",
    BlockSource* bs, Block* bl, BlockPos* bp, unsigned __int8 a4, Actor* pl, bool a6)
{
    IF_LISTENED(EVENT_TYPES::onPlaceBlock)
    {
        CallEventEx(EVENT_TYPES::onPlaceBlock, PlayerClass::newPlayer((Player*)pl), BlockClass::newBlock(bl,bp,bs));
    }
    IF_LISTENDED_END();
    return original(bs, bl, bp, a4, pl, a6);
}

// ===== onOpenContainer_Chest =====
THook(bool, "?use@ChestBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    void* _this, Player* pl , BlockPos* bp)
{
    IF_LISTENED(EVENT_TYPES::onOpenContainer)
    {
        int dim = Raw_GetPlayerDimId(pl);
        BlockSource* bs = Raw_GetBlockSourceByDim(dim);
        Block* bl = Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs);

        CallEventEx(EVENT_TYPES::onOpenContainer, PlayerClass::newPlayer(pl), BlockClass::newBlock(bl, bp, dim));
    }
    IF_LISTENDED_END();
    return original(_this, pl, bp);
}

// ===== onCloseContainer_Chest =====
class ChestBlockActor;
THook(bool, "?stopOpen@ChestBlockActor@@UEAAXAEAVPlayer@@@Z",
    ChestBlockActor* _this, Player* pl)
{
    IF_LISTENED(EVENT_TYPES::onCloseContainer)
    {
        auto bp = (BlockPos*)((char*)_this - 204);
        int dim = Raw_GetPlayerDimId(pl);
        BlockSource* bs = Raw_GetBlockSourceByDim(dim);
        Block* bl = Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs);

        CallEventEx(EVENT_TYPES::onCloseContainer, PlayerClass::newPlayer(pl), BlockClass::newBlock(bl, bp, dim));
    }
    IF_LISTENDED_END();
    return original(_this, pl);
}

// ===== onOpenContainer_Barrel =====
THook(bool, "?use@BarrelBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    void* _this, Player* pl, BlockPos* bp)
{
    IF_LISTENED(EVENT_TYPES::onOpenContainer)
    {
        int dim = Raw_GetPlayerDimId(pl);
        BlockSource* bs = Raw_GetBlockSourceByDim(dim);
        Block* bl = Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs);

        CallEventEx(EVENT_TYPES::onOpenContainer, PlayerClass::newPlayer(pl), BlockClass::newBlock(bl, bp, dim));
    }
    IF_LISTENDED_END();
    return original(_this, pl, bp);
}

// ===== onCloseContainer_Barrel =====
THook(bool, "?stopOpen@BarrelBlockActor@@UEAAXAEAVPlayer@@@Z",
    void* _this, Player* pl)
{
    IF_LISTENED(EVENT_TYPES::onCloseContainer)
    {
        auto bp = (BlockPos*)((char*)_this - 204);
        int dim = Raw_GetPlayerDimId(pl);
        BlockSource* bs = Raw_GetBlockSourceByDim(dim);
        Block* bl = Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs);

        CallEventEx(EVENT_TYPES::onCloseContainer, PlayerClass::newPlayer(pl), BlockClass::newBlock(bl, bp, dim));
    }
    IF_LISTENDED_END();
    return original(_this, pl);
}

// ===== onContainerChange =====
class LevelContainerModel;
THook(void, "?_onItemChanged@LevelContainerModel@@MEAAXHAEBVItemStack@@0@Z",
    LevelContainerModel* _this, int slotNumber, ItemStack* oldItem, ItemStack* newItem)
{
    IF_LISTENED(EVENT_TYPES::onContainerChange)
    {
        Actor* pl = dAccess<Actor*>(_this, 208);
        BlockSource* bs = Raw_GetBlockSourceByActor(pl);
        BlockPos* bpos = (BlockPos*)((char*)_this + 216);
        Block* block = Raw_GetBlockByPos(bpos->x, bpos->y, bpos->z, bs);

        bool isPutIn = Raw_IsNull(oldItem);

        CallEvent(EVENT_TYPES::onContainerChange, PlayerClass::newPlayer((Player*)pl), BlockClass::newBlock(block, bpos, bs),
            slotNumber, isPutIn, ItemClass::newItem(isPutIn ? newItem : oldItem));
    }
    IF_LISTENDED_END();
    return original(_this, slotNumber, oldItem, newItem);
}

// ===== onOpenContainerScreen =====
THook(bool, "?canOpenContainerScreen@Player@@UEAA_NXZ",
    Player* a1)
{
    IF_LISTENED(EVENT_TYPES::onOpenContainerScreen)
    {
        CallEventEx(EVENT_TYPES::onOpenContainerScreen, PlayerClass::newPlayer(a1));
    }
    IF_LISTENDED_END();
    return original(a1);
}

// ===== onMobHurt =====
THook(bool, "?_hurt@Mob@@MEAA_NAEBVActorDamageSource@@H_N1@Z",
    Mob* ac, ActorDamageSource* ads, int damage, bool unk1_1, bool unk2_0)
{
    IF_LISTENED(EVENT_TYPES::onMobHurt)
    {
        if (ac)
        {
            auto level = offPlayer::getLevel(ac);
            char v83;
            auto v6 = *(void**)(*(__int64(__fastcall**)(void*, char*))(*(uintptr_t*)ads + 64))(ads, &v83);
            auto src = SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z", Actor*, Level*,
                void*, bool)(level, v6, 0);
            

            CallEventEx(EVENT_TYPES::onMobHurt, EntityClass::newEntity(ac), EntityClass::newEntity(src),
                Number::newNumber(damage));
        }
    }
    IF_LISTENDED_END();
    return original(ac, ads, damage, unk1_1, unk2_0);
}

// ===== onExplode =====
THook(bool, "?explode@Level@@UEAAXAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
    Level* _this, BlockSource* bs, Actor* actor, Vec3* pos, float a5, bool a6, bool a7, float a8, bool a9)
{
    IF_LISTENED(EVENT_TYPES::onExplode)
    {
        if (actor)
        {
            CallEventEx(EVENT_TYPES::onExplode, EntityClass::newEntity(actor), FloatPos::newPos(pos->x, pos->y, pos->z, Raw_GetEntityDimId(actor)));
        }
    }
    IF_LISTENDED_END();
    return original(_this, bs, actor, pos, a5, a6, a7, a8, a9);
}

// ===== onBlockExploded =====
THook(void, "?onExploded@Block@@QEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@@Z",
    Block* _this, BlockSource *bs, BlockPos *bp, Actor * actor)
{
    IF_LISTENED(EVENT_TYPES::onBlockExploded)
    {
        if (actor)
        {
            CallEvent(EVENT_TYPES::onBlockExploded, BlockClass::newBlock(_this, bp, bs), EntityClass::newEntity(actor));
        }
    }
    IF_LISTENDED_END();
    return original(_this, bs, bp, actor);
}

// ===== onProjectileHit =====
THook(void, "?onProjectileHit@Block@@QEBAXAEAVBlockSource@@AEBVBlockPos@@AEBVActor@@@Z",
    Block* _this, BlockSource* bs, BlockPos* bp, Actor* actor)
{
    IF_LISTENED(EVENT_TYPES::onProjectileHit)       //################# 击中实体时也会触发 ################# 
    {
        CallEvent(EVENT_TYPES::onProjectileHit, BlockClass::newBlock(_this,bp,bs), EntityClass::newEntity(actor));
    }
    IF_LISTENDED_END();
    return original(_this, bs, bp, actor);
}

// ===== onSplashPotionHitEffect =====
/*
THook(void, "?doOnHitEffect@SplashPotionEffectSubcomponent@@UEAAXAEAVActor@@AEAVProjectileComponent@@@Z",
    void* _this, Actor* a2, ProjectileComponent* a3)
{
    IF_LISTENED(EVENT_TYPES::onSplashPotionHitEffect)
    {
        auto uniqueId = (ActorUniqueID*)((uintptr_t)a3 + 8);
        auto ac = Raw_GetEntityByUniqueId(*uniqueId);
        CallEvent(EVENT_TYPES::onSplashPotionHitEffect, EntityClass::newEntity(a2), EntityClass::newEntity(ac));
    }
    IF_LISTENDED_END();
    // 不original即可拦截，等待CallEvent重写...
    // Event -> [1]被喷溅药水效果影响的实体对象 [2]投掷喷溅药水的实体对象

    original(_this, a2, a3);
}
*/

// ===== onBlockInteracted =====
THook(unsigned short, "?onBlockInteractedWith@VanillaServerGameplayEventListener@@UEAA?AW4EventResult@@AEAVPlayer@@AEBVBlockPos@@@Z",
    void* _this, Player* pl, BlockPos* bp)
{
    IF_LISTENED(EVENT_TYPES::onBlockInteracted)
    {
        BlockSource* bs = Raw_GetBlockSourceByActor((Actor*)pl);

        CallEventRtn(EVENT_TYPES::onBlockInteracted, 0, PlayerClass::newPlayer(pl),
            BlockClass::newBlock(Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs), bp, bs));
    }
    IF_LISTENDED_END();
    return original(_this, pl, bp);
}

// ===== onUseRespawnAnchor =====
THook(bool, "?trySetSpawn@RespawnAnchorBlock@@CA_NAEAVPlayer@@AEBVBlockPos@@AEAVBlockSource@@AEAVLevel@@@Z",
    Player* pl, BlockPos* a2, BlockSource* a3, Level* a4)
{
    IF_LISTENED(EVENT_TYPES::onUseRespawnAnchor)
    {
        CallEventEx(EVENT_TYPES::onUseRespawnAnchor,PlayerClass::newPlayer(pl),IntPos::newPos(*a2, Raw_GetBlockDimension(a3)));
    }
    IF_LISTENDED_END();
    return original(pl, a2, a3, a4);
}

// ===== onFarmLandDecay =====
THook(void, "?transformOnFall@FarmBlock@@UEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@M@Z",
    void *_this, BlockSource * bs, BlockPos * bp, Actor * ac, float a5)
{
    IF_LISTENED(EVENT_TYPES::onFarmLandDecay)
    {
        CallEvent(EVENT_TYPES::onFarmLandDecay,IntPos::newPos(*bp, Raw_GetBlockDimension(bs)),EntityClass::newEntity(ac));
    }
    IF_LISTENDED_END();
    return original(_this,bs,bp,ac,a5);
}

// ===== onUseFrameBlock =====
THook(bool, "?use@ItemFrameBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    void* _this, Player* a2, BlockPos* a3)
{
    IF_LISTENED(EVENT_TYPES::onUseFrameBlock)
    {
        BlockSource * bs = Raw_GetBlockSourceByActor((Actor*)a2);
        Block* bl = Raw_GetBlockByPos(a3->x, a3->y, a3->z, bs);
        CallEventEx(EVENT_TYPES::onUseFrameBlock, PlayerClass::newPlayer(a2), BlockClass::newBlock(bl, a3, bs));
    }
    IF_LISTENDED_END();
    return original(_this, a2, a3);
}

THook(bool, "?attack@ItemFrameBlock@@UEBA_NPEAVPlayer@@AEBVBlockPos@@@Z",
    void* _this, Player* a2, BlockPos* a3)
{
    IF_LISTENED(EVENT_TYPES::onUseFrameBlock)
    {
        BlockSource* bs = Raw_GetBlockSourceByActor((Actor*)a2);
        Block* bl = Raw_GetBlockByPos(a3->x, a3->y, a3->z, bs);
        CallEventEx(EVENT_TYPES::onUseFrameBlock, PlayerClass::newPlayer(a2), BlockClass::newBlock(bl, a3, bs));
    }
    IF_LISTENDED_END();
    return original(_this, a2, a3);
}

// ===== onPistonPush =====
THook(bool, "?_attachedBlockWalker@PistonBlockActor@@AEAA_NAEAVBlockSource@@AEBVBlockPos@@EE@Z",
    BlockActor* _this, BlockSource* bs, BlockPos* bp, unsigned a3, unsigned a4)
{
    IF_LISTENED(EVENT_TYPES::onPistonPush)
    {
        int dim = Raw_GetBlockDimension(bs);
        BlockPos pistonPos = _this->getPosition();
        Block* pushedBlock = Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs);

        CallEventEx(EVENT_TYPES::onPistonPush, IntPos::newPos(pistonPos, dim), BlockClass::newBlock(pushedBlock, bp, dim));
    }
    IF_LISTENDED_END();
    return original(_this, bs, bp, a3, a4);
}

// ===== onHopperSearchItem =====
THook(bool, "?_tryPullInItemsFromAboveContainer@Hopper@@IEAA_NAEAVBlockSource@@AEAVContainer@@AEBVVec3@@@Z",
    void* _this, BlockSource* bs, void* container, Vec3* pos)
{
    IF_LISTENED(EVENT_TYPES::onHopperSearchItem)
    {
        CallEventEx(EVENT_TYPES::onHopperSearchItem, FloatPos::newPos(*pos, Raw_GetBlockDimension(bs)));
    }
    IF_LISTENDED_END();
    return original(_this, bs, container, pos);
}

// ===== onHopperPushOut =====
THook(bool, "?_pushOutItems@Hopper@@IEAA_NAEAVBlockSource@@AEAVContainer@@AEBVVec3@@H@Z",
    void* _this, BlockSource* bs, void* container, Vec3* pos, int a5)
{
    IF_LISTENED(EVENT_TYPES::onHopperPushOut)
    {
        CallEventEx(EVENT_TYPES::onHopperPushOut, FloatPos::newPos(*pos, Raw_GetBlockDimension(bs)));
    }
    IF_LISTENDED_END();
    return original(_this, bs, container, pos, a5);
}

// ===== onFireSpread =====
THook(bool, "?_trySpawnBlueFire@FireBlock@@AEBA_NAEAVBlockSource@@AEBVBlockPos@@@Z",
    void* _this, BlockSource* bs, BlockPos* bp)
{
    IF_LISTENED(EVENT_TYPES::onFireSpread)
    {
        CallEventEx(EVENT_TYPES::onFireSpread, IntPos::newPos(*bp, Raw_GetBlockDimension(bs)));
    }
    IF_LISTENDED_END();
    return original(_this, bs, bp);
}

// ===== onFishingHookRetrieve =====

THook(__int64, "?retrieve@FishingHook@@QEAAHXZ",
    FishingHook* _this)
{
    IF_LISTENED(EVENT_TYPES::onFishingHookRetrieve)
    {
        auto pl = (Player*)Raw_GetFishingHookOwner(_this);
        auto fh = (Actor*)_this;
        CallEventRtn(EVENT_TYPES::onFishingHookRetrieve, 0i64, PlayerClass::newPlayer(pl), EntityClass::newEntity(fh));
    }
    IF_LISTENDED_END();
    return original(_this);
}

// ===== onPlayerCmd & onConsoleCmd =====
THook(bool, "?executeCommand@MinecraftCommands@@QEBA?AUMCRESULT@@V?$shared_ptr@VCommandContext@@@std@@_N@Z",
    MinecraftCommands* _this, unsigned int* a2, std::shared_ptr<CommandContext> x, char a4)
{
    Player* player = MakeSP(x->getOrigin());
    string cmd = x->getCmd();
    if (cmd.front() == '/')
        cmd = cmd.substr(1);
    if (!cmd.empty())
    {
        if (player)
        {
            // Player Command
            vector<string> paras;
            string prefix = LxlFindCmdReg(true, cmd, paras);

            if (!prefix.empty())
            {
                //found
                int perm = localShareData->playerCmdCallbacks[prefix].perm;

                if (Raw_GetPlayerPermLevel(player) >= perm)
                {
                    bool callbackRes = CallPlayerCmdCallback(player, prefix, paras);
                    IF_LISTENED(EVENT_TYPES::onPlayerCmd)
                    {
                        CallEvent(EVENT_TYPES::onPlayerCmd, PlayerClass::newPlayer(player), cmd);
                    }
                    IF_LISTENDED_END();
                    if (!callbackRes)
                        return false;
                }
            }
        }
        else
        {
            // PreProcess
            if (!ProcessDebugEngine(cmd))
                return false;
            ProcessStopServer(cmd);
            ProcessHotManagement(cmd);

            //CallEvents
            vector<string> paras;
            string prefix = LxlFindCmdReg(false, cmd, paras);

            if (!prefix.empty())
            {
                bool callbackRes = CallServerCmdCallback(prefix,paras);
                IF_LISTENED(EVENT_TYPES::onConsoleCmd)
                {
                    CallEventEx(EVENT_TYPES::onConsoleCmd, cmd);
                }
                IF_LISTENDED_END();
                if (!callbackRes)
                    return false;
            }
        }
    }
    return original(_this, a2, x, a4);
}

// ===== onFormSelected =====
THook(void, "?handle@?$PacketHandlerDispatcherInstance@VModalFormResponsePacket@@$0A@@@UEBAXAEBVNetworkIdentifier@@AEAVNetEventCallback@@AEAV?$shared_ptr@VPacket@@@std@@@Z",
    void* _this, NetworkIdentifier* id, ServerNetworkHandler* handler, void* pPacket)
{
    //IF_LISTENED(EVENT_TYPES::onFormSelected)
    Packet* packet = *(Packet**)pPacket;
    Player* p = Raw_GetPlayerFromPacket(handler, id, packet);

    if (p)
    {
        unsigned formId = dAccess<unsigned>(packet, 48);
        string data = dAccess<string>(packet,56);

        if (data.back() == '\n')
            data.pop_back();
          
        CallFormCallback(p, formId, data);
        // No CallEvent here
    }

    original(_this, id, handler, pPacket);
}

// ===== onScoreChanged =====
THook(void, "?onScoreChanged@ServerScoreboard@@UEAAXAEBUScoreboardId@@AEBVObjective@@@Z",
    Scoreboard* _this, ScoreboardId* a1, Objective* a2)
{
    IF_LISTENED(EVENT_TYPES::onScoreChanged)
    {
        int id = a1->id;

        Player* player = nullptr;
        auto pls = Raw_GetOnlinePlayers();
        for (auto& pl : pls)
        {
            if (globalScoreBoard->getScoreboardId(*(Actor*)pl).id == id)
            {
                player = pl;
                break;
            }
        }

        if (player)
        {
            CallEvent(EVENT_TYPES::onScoreChanged, PlayerClass::newPlayer(player), Number::newNumber(a2->getPlayerScore(*a1).getCount()),
                String::newString(a2->getName()), String::newString(a2->getDisplayName()));
        }
    }
    IF_LISTENDED_END();

    return original(_this, a1, a2);
}

// ===== onConsoleOutput =====
THook(ostream&, "??$_Insert_string@DU?$char_traits@D@std@@_K@std@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@0@AEAV10@QEBD_K@Z",
    ostream& _this, const char* str, unsigned size)
{
    if (&_this == &cout)
    {
        IF_LISTENED(EVENT_TYPES::onConsoleOutput)
        {
            CallEventRtn(EVENT_TYPES::onConsoleOutput, _this, String::newString(string(str)));
        }
        IF_LISTENDED_END();
    }
    return original(_this, str, size);
}
