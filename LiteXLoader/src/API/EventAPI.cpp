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
#include "EngineOwnData.h"
#include "EngineGlobalData.h"
#include "APIhelp.h"
#include "BlockAPI.h"
#include "ItemAPI.h"
#include "EntityAPI.h"
#include "PlayerAPI.h"
#include <Configs.h>
using namespace std;
using namespace script;

//////////////////// Helper Funcs ////////////////////

bool ProcessDebugEngine(const string& cmd);
bool CallPlayerCmdCallback(Player *player, const string& cmd);
bool CallServerCmdCallback(const string& cmd);
bool CallFormCallback(unsigned formId, const string& data);

//////////////////// Listeners ////////////////////

enum class EVENT_TYPES : int
{
    onJoin=0, onLeft, onPlayerCmd, onChat, 
    onRespawn, onChangeDim, onJump, onSneak, onAttack, onEat, onMove, onSetArmor,
    onUseItem, onTakeItem, onDropItem,
    onDestroyingBlock, onDestroyBlock, onPlaceBlock,
    onOpenContainer, onCloseContainer, onContainerChangeSlot,
    onMobDie, onMobHurt, onExplode, onBlockExploded, onCmdBlockExecute,
    onProjectileHit, onBlockInteractd, onUseRespawnAnchor, onFarmLandDecay,
    onPistonPush, onHopperSearchItem, onHopperPushOut, onFireSpread, 
    onServerStarted, onConsoleCmd, onFormSelected, onConsoleOutput,
    EVENT_COUNT
};
static const std::unordered_map<string, EVENT_TYPES> EventsMap{
    {"onJoin",EVENT_TYPES::onJoin},
    {"onLeft",EVENT_TYPES::onLeft},
    {"onPlayerCmd",EVENT_TYPES::onPlayerCmd},
    {"onChat",EVENT_TYPES::onChat},
    {"onRespawn",EVENT_TYPES::onRespawn},
    {"onChangeDim",EVENT_TYPES::onChangeDim},
    {"onJump",EVENT_TYPES::onJump},
    {"onSneak",EVENT_TYPES::onSneak},
    {"onAttack",EVENT_TYPES::onAttack},
    {"onEat",EVENT_TYPES::onEat},
    {"onMove",EVENT_TYPES::onMove},
    {"onSetArmor",EVENT_TYPES::onSetArmor},
    {"onMobDie",EVENT_TYPES::onMobDie},
    {"onMobHurt",EVENT_TYPES::onMobHurt},
    {"onUseItem",EVENT_TYPES::onUseItem},
    {"onTakeItem",EVENT_TYPES::onTakeItem},
    {"onDropItem",EVENT_TYPES::onDropItem},
    {"onDestroyingBlock",EVENT_TYPES::onDestroyingBlock},
    {"onDestroyBlock",EVENT_TYPES::onDestroyBlock},
    {"onPlaceBlock",EVENT_TYPES::onPlaceBlock},
    {"onExplode",EVENT_TYPES::onExplode},
    {"onBlockExploded",EVENT_TYPES::onBlockExploded},
    {"onOpenContainer",EVENT_TYPES::onOpenContainer},
    {"onCloseContainer",EVENT_TYPES::onCloseContainer},
    {"onContainerChangeSlot",EVENT_TYPES::onContainerChangeSlot},
    {"onCmdBlockExecute",EVENT_TYPES::onCmdBlockExecute},
    {"onProjectileHit",EVENT_TYPES::onProjectileHit},
    {"onBlockInteractd",EVENT_TYPES::onBlockInteractd},
    {"onUseRespawnAnchor",EVENT_TYPES::onUseRespawnAnchor},
    {"onFarmLandDecay",EVENT_TYPES::onFarmLandDecay},
    {"onPistonPush",EVENT_TYPES::onPistonPush},
    {"onHopperSearchItem",EVENT_TYPES::onHopperSearchItem},
    {"onHopperPushOut",EVENT_TYPES::onHopperPushOut},
    {"onFireSpread",EVENT_TYPES::onFireSpread},
    {"onServerStarted",EVENT_TYPES::onServerStarted},
    {"onConsoleCmd",EVENT_TYPES::onConsoleCmd},
    {"onConsoleOutput",EVENT_TYPES::onConsoleOutput},
    {"onFormSelected",EVENT_TYPES::onFormSelected},
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
            ERRPRINT(e.message()); \
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
            ERRPRINT(e.message()); \
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
            ERRPRINT(e.message()); \
        } \
    }\
    if(!passToBDS) { return RETURN_VALUE; }


//////////////////// APIs ////////////////////

Local<Value> Listen(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kFunction)

    try{
        int eventId = int(EventsMap.at(args[0].asString().toString()));
        Global<Function> func{args[1].asFunction()};
        listenerList[eventId].push_back({EngineScope::currentEngine(),func});
        return Boolean::newBoolean(true);
    }
    catch(const std::logic_error& e)
    {
        ERROR("Event \""+ args[0].asString().toString() +"\" No Found!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail to bind listener!")
}

//注册后台调试命令
void RegisterBuiltinCmds()
{
    Raw_RegisterCmd(LXL_DEBUG_CMD, string(LXL_SCRIPT_LANG_TYPE) + " Engine Real-time Debugging", 4);
    DEBUG("Builtin Cmds Registered.");
}


//////////////////// Hook ////////////////////

#define IF_LISTENED(EVENT) if(!listenerList[int(EVENT)].empty())

void InitEventListeners()
{

// ===== onChat =====
    Event::addEventListener([](ChatEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onChat)
        {
            CallEventEx(EVENT_TYPES::onChat, PlayerClass::newPlayer(ev.pl), ev.msg);
        }
        return true;
    });

// ===== onChangeDimension =====
    Event::addEventListener([](ChangeDimEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onChangeDim)
        {
            CallEvent(EVENT_TYPES::onChangeDim, PlayerClass::newPlayer(ev.Player));
        }
    });

// ===== onCmdBlockExecute =====
    Event::addEventListener([](CmdBlockExeEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onCmdBlockExecute)
        {
            CallEventEx(EVENT_TYPES::onCmdBlockExecute, ev.cmd, IntPos::newPos(ev.bpos.x, ev.bpos.y, ev.bpos.z));
        }
        return true;
    });

// ===== onMobHurt =====
    Event::addEventListener([](MobHurtedEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onMobHurt)
        {
            auto level = offPlayer::getLevel(ev.Mob);
            auto source = SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z"
                , Actor*, Level*, ActorDamageSource*, bool)(level, ev.ActorDamageSource, 0);

            CallEventEx(EVENT_TYPES::onMobHurt, EntityClass::newEntity(ev.Mob), EntityClass::newEntity(source),
                Number::newNumber(ev.Damage));
        }
        return true;
    });

// ===== onMobDie =====
    Event::addEventListener([](MobDieEV ev)
    {
        IF_LISTENED(EVENT_TYPES::onMobDie)
        {
            CallEventEx(EVENT_TYPES::onMobDie, EntityClass::newEntity(ev.mob), EntityClass::newEntity(ev.DamageSource));
        }
        return true;
    });

// For RegisterCmd...
    Event::addEventListener([](RegCmdEV ev)
    {
        extern CommandRegistry* CmdReg;
        CmdReg = ev.CMDRg;

        //注册队列中的命令
        extern std::vector<tuple<string, string, int>> toRegCmdQueue;
        for(auto & cmdData : toRegCmdQueue)
        {
            Raw_RegisterCmd(std::get<0>(cmdData), std::get<1>(cmdData), std::get<2>(cmdData));
        }
        toRegCmdQueue.clear();
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

            IF_LISTENED(EVENT_TYPES::onServerStarted)
            {
                CallEvent(EVENT_TYPES::onServerStarted);
            }
        }
    });
}

// ===== onJoin =====
THook(bool, "?_loadNewPlayer@ServerNetworkHandler@@AEAA_NAEAVServerPlayer@@_N@Z",
    ServerNetworkHandler* _this, ServerPlayer* pl, bool a3)
{
    IF_LISTENED(EVENT_TYPES::onJoin)
    {
        CallEvent(EVENT_TYPES::onJoin, PlayerClass::newPlayer(pl));
    }
    return original(_this, pl, a3);
}

// ===== onLeft =====
THook(void, "?_onPlayerLeft@ServerNetworkHandler@@AEAAXPEAVServerPlayer@@_N@Z",
    ServerNetworkHandler* _this, Player* pl, char a3)
{
    IF_LISTENED(EVENT_TYPES::onLeft)
    {
        CallEvent(EVENT_TYPES::onLeft, PlayerClass::newPlayer(pl));
    }
    return original(_this, pl, a3);
}

Level* level;
ActorUniqueID aid;

// ===== onAttack =====
THook(bool, "?attack@Player@@UEAA_NAEAVActor@@@Z",
    Player* pl,  Actor* ac)
{
    IF_LISTENED(EVENT_TYPES::onAttack)
    {
        CallEventEx(EVENT_TYPES::onAttack, PlayerClass::newPlayer(pl), EntityClass::newEntity(ac));
    }
    return original(pl, ac);
}

// ===== onEat =====
THook(void, "?eat@Player@@QEAAXAEBVItemStack@@@Z",
    Player* _this, ItemStack* eaten)
{
    IF_LISTENED(EVENT_TYPES::onEat)
    {
        CallEvent(EVENT_TYPES::onEat, PlayerClass::newPlayer(_this), ItemClass::newItem(eaten));
    }
    return original(_this, eaten);
}

// ===== onMove =====
THook(void, "??0MovePlayerPacket@@QEAA@AEAVPlayer@@AEBVVec3@@@Z",
    void *_this, Player * pl, Vec3 * to)
{
    IF_LISTENED(EVENT_TYPES::onMove)
    {
        CallEvent(EVENT_TYPES::onMove, PlayerClass::newPlayer(pl), FloatPos::newPos(*to, Raw_GetPlayerDimId(pl)));
    }
    return original(_this, pl, to);
}

// ===== onSetArmor =====
THook(void, "?setArmor@Player@@UEAAXW4ArmorSlot@@AEBVItemStack@@@Z",
    Player* _this, unsigned slot, ItemStack* it)
{
    IF_LISTENED(EVENT_TYPES::onSetArmor)
    {
        CallEvent(EVENT_TYPES::onSetArmor, PlayerClass::newPlayer(_this), Number::newNumber((int)slot), ItemClass::newItem(it));
    }
    return original(_this, slot, it);
}

// ===== onRespawn =====
THook(bool, "?respawn@Player@@UEAAXXZ",
    Player* pl)
{
    IF_LISTENED(EVENT_TYPES::onRespawn)
    {
        CallEventEx(EVENT_TYPES::onRespawn, PlayerClass::newPlayer(pl));
    }
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
    return original(_this, actor, a2, a3);
}

// ===== onPlayerCmd =====
THook(void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
    ServerNetworkHandler* _this, NetworkIdentifier* id, Packet* pkt)
{
    Player* player = Raw_GetPlayerFromPacket(_this, id, pkt);

    if (player)
    {   
        // Player Command
        auto cmd = std::string(*(std::string*)((uintptr_t)pkt + 48));
        if (cmd.front() == '/')
            cmd = cmd.substr(1);
     
        bool callbackRes = CallPlayerCmdCallback(player,cmd);
        IF_LISTENED(EVENT_TYPES::onPlayerCmd)
        {
            CallEvent(EVENT_TYPES::onPlayerCmd, PlayerClass::newPlayer(player), cmd);
        }
        if (!callbackRes)
            return;
    }
    
    return original(_this, id, pkt);
}

// ===== onUseItem =====
THook(bool, "?useItemOn@GameMode@@UEAA_NAEAVItemStack@@AEBVBlockPos@@EAEBVVec3@@PEBVBlock@@@Z",
    void* _this,  ItemStack* item, BlockPos* bp, unsigned __int8 a4, Vec3* a5,  Block* bl)
{
    IF_LISTENED(EVENT_TYPES::onUseItem)
    {
        auto sp = *reinterpret_cast<Player**>(reinterpret_cast<unsigned long long>(_this) + 8);

        CallEventEx(EVENT_TYPES::onUseItem, PlayerClass::newPlayer(sp), ItemClass::newItem(item),
            BlockClass::newBlock(bl, bp, WPlayer(*sp).getDimID()));
    }
    return original(_this, item, bp, a4, a5, bl);
}

// ===== onDestroyingBlock =====
THook(float, "?getDestroySpeed@Player@@QEBAMAEBVBlock@@@Z",
    Player* _this, Block* bl)
{
    IF_LISTENED(EVENT_TYPES::onDestroyingBlock)     //############## 找个可以获取坐标的？ ################
    {
        CallEvent(EVENT_TYPES::onDestroyingBlock, PlayerClass::newPlayer(_this), BlockClass::newBlock(bl));
    }
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
    return original(_this, pl, pos,a3, a4);
}

// ===== onPlaceBlock =====
THook(bool, "?mayPlace@BlockSource@@QEAA_NAEBVBlock@@AEBVBlockPos@@EPEAVActor@@_N@Z",
    BlockSource* bs, Block* bl, BlockPos* bp, unsigned __int8 a4, Actor* pl, bool a6)
{
    IF_LISTENED(EVENT_TYPES::onPlaceBlock)
    {
        CallEventEx(EVENT_TYPES::onPlaceBlock, PlayerClass::newPlayer((Player*)pl), BlockClass::newBlock(bl,bp,bs));
    }
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
    return original(_this, pl);
}

// ===== onContainerChangeSlot =====
class LevelContainerModel;
THook(void, "?_onItemChanged@LevelContainerModel@@MEAAXHAEBVItemStack@@0@Z",
    LevelContainerModel* _this, int slotNumber, ItemStack* oldItem, ItemStack* newItem)
{
    IF_LISTENED(EVENT_TYPES::onContainerChangeSlot)
    {
        Actor* pl = dAccess<Actor*>(_this, 208);
        BlockSource* bs = Raw_GetBlockSourceByActor(pl);
        BlockPos* bpos = (BlockPos*)((char*)_this + 216);
        Block* block = Raw_GetBlockByPos(bpos->x, bpos->y, bpos->z, bs);

        bool isPutIn = Raw_IsNull(oldItem);

        CallEvent(EVENT_TYPES::onContainerChangeSlot, PlayerClass::newPlayer((Player*)pl), BlockClass::newBlock(block, bpos, bs),
            slotNumber, isPutIn, ItemClass::newItem(isPutIn ? newItem : oldItem));
    }
    return original(_this, slotNumber, oldItem, newItem);
}

// ===== onExplode =====
THook(bool, "?explode@Level@@UEAAXAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
    Level* _this, BlockSource* bs, Actor* actor, Vec3 pos, float a5, bool a6, bool a7, float a8, bool a9)
{
    IF_LISTENED(EVENT_TYPES::onExplode)
    {
        CallEventEx(EVENT_TYPES::onExplode, EntityClass::newEntity(actor), FloatPos::newPos(pos.x, pos.y, pos.z));
    }
    return original(_this, bs, actor, pos, a5, a6, a7, a8, a9);
}

// ===== onBlockExploded =====
THook(void, "?onExploded@Block@@QEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@@Z",
    Block* _this, BlockSource *bs, BlockPos *bp, Actor * actor)
{
    IF_LISTENED(EVENT_TYPES::onBlockExploded)
    {
        CallEvent(EVENT_TYPES::onBlockExploded, BlockClass::newBlock(_this,bp,bs), EntityClass::newEntity(actor));
    }
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
    return original(_this, bs, bp, actor);
}

// ===== onBlockInteractd =====
THook(unsigned short, "?onBlockInteractedWith@VanillaServerGameplayEventListener@@UEAA?AW4EventResult@@AEAVPlayer@@AEBVBlockPos@@@Z",
    void* _this, Player* pl, BlockPos* bp)
{
    IF_LISTENED(EVENT_TYPES::onBlockInteractd)
    {
        BlockSource* bs = Raw_GetBlockSourceByActor((Actor*)pl);

        CallEventRtn(EVENT_TYPES::onBlockInteractd, 0, PlayerClass::newPlayer(pl), 
            BlockClass::newBlock(Raw_GetBlockByPos(bp->x, bp->y, bp->z, bs), bp, bs));
    }
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
    return original(_this,bs,bp,ac,a5);
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
    return original(_this, bs, bp);
}

// ===== onConsoleCmd =====
THook(bool, "?executeCommand@MinecraftCommands@@QEBA?AUMCRESULT@@V?$shared_ptr@VCommandContext@@@std@@_N@Z",
    MinecraftCommands* _this, unsigned int* a2, std::shared_ptr<CommandContext> x, char a4)
{
    Player* player = MakeSP(x->getOrigin());
    if (!player)
    {
        // Server Command
        string cmd = x->getCmd();
        if (cmd.front() == '/')
            cmd = cmd.substr(1);

        if (!ProcessDebugEngine(cmd))
            return false;

        bool callbackRes = CallServerCmdCallback(cmd);
        IF_LISTENED(EVENT_TYPES::onConsoleCmd)
        {
            CallEventEx(EVENT_TYPES::onConsoleCmd, cmd);
        }
        if (!callbackRes)
            return false;
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
          
        CallFormCallback(formId, data);
        // No CallEvent here
    }

    original(_this, id, handler, pPacket);
}

// ===== onConsoleOutput =====
THook(ostream&, "??$_Insert_string@DU?$char_traits@D@std@@_K@std@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@0@AEAV10@QEBD_K@Z",
    ostream& _this, const char* str, unsigned size)
{
    IF_LISTENED(EVENT_TYPES::onConsoleOutput)
    {
        CallEventRtn(EVENT_TYPES::onConsoleOutput, _this, String::newString(string(str)));
    }
    return original(_this, str, size);
}


//////////////////// Helper Funcs ////////////////////

vector<string> SplitCmdParas(const string& paras)
{
    vector<string> res;
    string now, strInQuote = "";
    istringstream strIn(paras);
    while (strIn >> now)
    {
        if (!strInQuote.empty())
        {
            strInQuote = strInQuote + " " + now;
            if (now.back() == '\"')
            {
                strInQuote.pop_back();
                res.push_back(strInQuote.erase(0, 1));
                strInQuote = "";
            }
        }
        else
        {
            if (now.front() == '\"')
                strInQuote = now;
            else
                res.push_back(now);
        }
    }
    if (!strInQuote.empty())
    {
        istringstream leftIn(strInQuote);
        while (leftIn >> now)
            res.push_back(now);
    }
    return res;
}

bool ProcessDebugEngine(const string& cmd)
{
    #define OUTPUT_DEBUG_SIGN() std::cout << "LiteXLoader-" << LXL_SCRIPT_LANG_TYPE << ">" << std::flush
    extern bool globalDebug;
    extern std::shared_ptr<ScriptEngine> debugEngine;

    if (cmd == LXL_DEBUG_CMD)
    {
        if (globalDebug)
        {   
            //EndDebug
            INFO("Debug mode ended");
            globalDebug = false;
        }
        else
        {   
            //StartDebug
            INFO("Debug mode begin");
            globalDebug = true;
            OUTPUT_DEBUG_SIGN();
        }
        return false;
    }
    if (globalDebug)
    {
        EngineScope enter(debugEngine.get());
        try
        {
            if (cmd == "stop")
            {
                WARN("请先退出Debug实时调试模式再使用stop！");
            }
            else
            {
                auto result = debugEngine->eval(cmd);
                PrintValue(std::cout, result);
                cout << endl;
                OUTPUT_DEBUG_SIGN();
            }
        }
        catch (Exception& e)
        {
            ERRPRINT(e);
            OUTPUT_DEBUG_SIGN();
        }
        return false;
    }
    return true;
}

bool CallPlayerCmdCallback(Player* player, const string& cmd)
{
    bool passToOriginalCmdEvent = true;

    for (auto engine : lxlModules)
    {
        EngineScope enter(engine.get());
        auto funcs = &(ENGINE_OWN_DATA()->playerCmdCallbacks);
        if (!funcs->empty())
            for (auto iter = funcs->begin(); iter != funcs->end(); ++iter)
            {
                string prefix = iter->first;
                if (cmd == prefix || (cmd.find_first_of(prefix) == 0 && cmd[prefix.size()] == ' '))
                    //如果命令与注册前缀全匹配，或者目标前缀后面为空格
                {
                    //Matched
                    Local<Array> args = Array::newArray();
                    if (cmd.size() > prefix.size())
                    {
                        //除了注册前缀之外还有额外参数
                        auto paras = SplitCmdParas(cmd.substr(prefix.size() + 1));
                        for (string para : paras)
                            args.add(String::newString(para));
                    }

                    auto res = iter->second.get().call({}, PlayerClass::newPlayer(player), args);
                    if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
                        passToOriginalCmdEvent = false;
                    break;
                }
            }
    }
    return passToOriginalCmdEvent;
}

bool CallServerCmdCallback(const string& cmd)
{
    bool passToOriginalCmdEvent = true;

    for (auto engine : lxlModules)
    {
        EngineScope enter(engine.get());
        auto funcs = &(ENGINE_OWN_DATA()->consoleCmdCallbacks);
        if (!funcs->empty())
            for (auto iter = funcs->begin(); iter != funcs->end(); ++iter)
            {
                string prefix = iter->first;
                if (cmd == prefix || (cmd.find_first_of(prefix) == 0 && cmd[prefix.size()] == ' '))
                    //如果命令与注册前缀全匹配，或者目标前缀后面为空格
                {
                    //Matched
                    Local<Array> args = Array::newArray();
                    if (cmd.size() > prefix.size())
                    {
                        //除了注册前缀之外还有额外参数
                        auto paras = SplitCmdParas(cmd.substr(prefix.size() + 1));
                        for (string para : paras)
                            args.add(String::newString(para));
                    }

                    auto res = iter->second.get().call({}, args);
                    if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
                        passToOriginalCmdEvent = false;
                    break;
                }
            }
    }
    return passToOriginalCmdEvent;
}

bool CallFormCallback(unsigned formId, const string& data)
{
    bool passToBDS = true;

    FormCallbackKey key{ LXL_SCRIPT_LANG_TYPE,formId };
    try
    {
        auto callback = engineGlobalData->formCallbacks.at(key);

        EngineScope scope(callback.engine);
        auto res = callback.func.get().call({}, JsonToValue(data));
        if (res.isNull() || (res.isBoolean() && res.asBoolean().value() == false))
            passToBDS = false;
    }
    catch (...)
    {
        ;
    }

    return passToBDS;
}