#include "EventAPI.h"
#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <cstdarg>
#include "APIhelp.h"
#include "../Configs.h"
#include "SymbolsHelper.h"
using namespace script;

//////////////////// Listeners ////////////////////

enum class EVENT_TYPES : int
{
    OnJoin=0, OnLeft, OnRespawn, OnChangeDim,
    OnPlayerCmd, OnChat, OnAttack,
    OnUseItem, OnTakeItem, OnDropItem,
    OnDestroyBlock, OnPlaceBlock,
    OnOpenChest, OnCloseChest, OnOpenBarrel, OnCloseBarrel, OnChangeSlot,
    OnMobDie, OnMobHurt, OnExplode, OnCmdBlockExecute,
    OnServerStarted, OnServerCmd,
    EVENT_COUNT
};
static const std::unordered_map<string, EVENT_TYPES> EventsMap{
    {"onPlayerJoin",EVENT_TYPES::OnJoin},
    {"onPlayerLeft",EVENT_TYPES::OnLeft},
    {"onRespawn",EVENT_TYPES::OnRespawn},
    {"onChangeDimension",EVENT_TYPES::OnChangeDim},
    {"onPlayerCmd",EVENT_TYPES::OnPlayerCmd},
    {"onChat",EVENT_TYPES::OnChat},
    {"onAttack",EVENT_TYPES::OnAttack},
    {"onMobDie",EVENT_TYPES::OnMobDie},
    {"onMobHurt",EVENT_TYPES::OnMobHurt},
    {"onUseItem",EVENT_TYPES::OnUseItem},
    {"onTakeItem",EVENT_TYPES::OnTakeItem},
    {"onDropItem",EVENT_TYPES::OnDropItem},
    {"onDestroyBlock",EVENT_TYPES::OnDestroyBlock},
    {"onPlaceBlock",EVENT_TYPES::OnPlaceBlock},
    {"onExplode",EVENT_TYPES::OnExplode},
    {"onOpenChest",EVENT_TYPES::OnOpenChest},
    {"onCloseChest",EVENT_TYPES::OnCloseChest},
    {"onOpenBarrel",EVENT_TYPES::OnOpenBarrel},
    {"onCloseBarrel",EVENT_TYPES::OnCloseBarrel},
    {"onChangeSlot",EVENT_TYPES::OnChangeSlot},
    {"onCmdBlockExecute",EVENT_TYPES::OnCmdBlockExecute},
    {"onServerStarted",EVENT_TYPES::OnServerStarted},
    {"onServerCmd",EVENT_TYPES::OnServerCmd},
};
struct ListenerListType
{
    ScriptEngine *engine;
    Global<Function> func;
};
static std::vector<ListenerListType> listenerList[int(EVENT_TYPES::EVENT_COUNT)];

// Call Event
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
#define CallEventEx(TYPE,...) \
    std::vector<ListenerListType> &nowList = listenerList[int(TYPE)]; \
    for(int i = 0; i < nowList.size(); ++i) { \
        EngineScope enter(nowList[i].engine); \
        try{ \
            auto result = nowList[i].func.get().call({},__VA_ARGS__); \
            if(result.isBoolean() && result.asBoolean().value() == false) \
                return false; \
        } \
        catch(const Exception& e) \
        { \
            ERROR("Event Callback Failed!"); \
            ERRPRINT(e.message()); \
        } \
    }


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
        DEBUG("Event \""+ args[0].asString().toString() +"\" No Found!\n");
        return Boolean::newBoolean(false);
    }
    CATCH("Fail to bind listener!")
}


void RegisterBuiltinCmds()
{
    //注册后台调试命令
    Hook_RegisterCmd(LXL_DEBUG_CMD, string(LXL_SCRIPT_LANG_TYPE) + " Engine Real-time Debugging", 4);

    DEBUG("Builtin Cmds Registered.");
}


//////////////////// Hook ////////////////////

#define IF_EXIST(EVENT) if(!listenerList[int(EVENT)].empty())

void RegisterEventListeners()
{
// ===== onPlayerJoin =====
    Event::addEventListener([](JoinEV ev)
    {
        IF_EXIST(EVENT_TYPES::OnJoin)
        {
            CallEvent(EVENT_TYPES::OnJoin, NewPlayer(ev.Player));
        }
    });

// ===== onPlayerLeft =====
    Event::addEventListener([](LeftEV ev)
    {
        IF_EXIST(EVENT_TYPES::OnLeft)
        {
            CallEvent(EVENT_TYPES::OnLeft, NewPlayer(ev.Player));
        }
    });

// ===== onChat =====
    Event::addEventListener([](ChatEV ev)
    {
        IF_EXIST(EVENT_TYPES::OnChat)
        {
            CallEventEx(EVENT_TYPES::OnChat, NewPlayer(ev.pl), ev.msg);
        }
        return true;
    });

// ===== onChangeDimension =====
    Event::addEventListener([](ChangeDimEV ev)
    {
        IF_EXIST(EVENT_TYPES::OnChangeDim)
        {
            CallEvent(EVENT_TYPES::OnChangeDim, NewPlayer(ev.Player));
        }
    });

// ===== onCmdBlockExecute =====
    Event::addEventListener([](CmdBlockExeEV ev)
    {
        IF_EXIST(EVENT_TYPES::OnCmdBlockExecute)
        {
            CallEventEx(EVENT_TYPES::OnCmdBlockExecute, ev.cmd, NewPos(ev.bpos.x, ev.bpos.y, ev.bpos.z));
        }
        return true;
    });

// ===== onServerStarted =====
    Event::addEventListener([](ServerStartedEV ev)
    {
        //注册预置命令
        RegisterBuiltinCmds();

        IF_EXIST(EVENT_TYPES::OnServerStarted)
        {
            CallEvent(EVENT_TYPES::OnServerStarted);
        }
    });

// For RegisterCmd...
    Event::addEventListener([](RegCmdEV ev) {
		CMDREG::SetCommandRegistry(ev.CMDRg);
    });
}

// ===== onAttack =====
THook(bool, "?attack@Player@@UEAA_NAEAVActor@@@Z",
    Player* pl,  Actor* ac)
{
    IF_EXIST(EVENT_TYPES::OnAttack)
    {
        CallEventEx(EVENT_TYPES::OnAttack, NewPlayer(pl), NewEntity(ac));
    }
    return original(pl, ac);
}

// ===== onRespawn =====
THook(bool, "?respawn@Player@@UEAAXXZ",
    Player* pl)
{
    IF_EXIST(EVENT_TYPES::OnRespawn)
    {
        CallEventEx(EVENT_TYPES::OnRespawn, NewPlayer(pl));
    }
    return original(pl);
}

// ===== onDropItem =====
THook(bool, "?drop@Player@@UEAA_NAEBVItemStack@@_N@Z",
    Player* _this, ItemStack* a2, bool a3)
{
    IF_EXIST(EVENT_TYPES::OnDropItem)
    {
        CallEventEx(EVENT_TYPES::OnDropItem, NewPlayer(_this), NewItem(a2));
    }
    return original(_this, a2, a3);
}

// ===== onTakeItem =====
THook(bool, "?take@Player@@QEAA_NAEAVActor@@HH@Z",
    Player* _this, Actor* actor, int a2, int a3)
{
    IF_EXIST(EVENT_TYPES::OnTakeItem)
    {
        CallEventEx(EVENT_TYPES::OnTakeItem, NewPlayer(_this), NewEntity(actor));
    }
    return original(_this, actor, a2, a3);
}

// ===== onPlayerCmd =====
THook(void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
    ServerNetworkHandler* _this, NetworkIdentifier* id, void* pkt)
{
    IF_EXIST(EVENT_TYPES::OnPlayerCmd)
    {
        auto player = SymCall("?_getServerPlayer@ServerNetworkHandler@@AEAAPEAVServerPlayer@@AEBVNetworkIdentifier@@E@Z",
            Player*, void*, void*, char)(_this, id, *(char*)((uintptr_t)pkt + 16));
        if (player)
        {   // Player Command
            auto cmd = std::string(*(std::string*)((uintptr_t)pkt + 48));
            if (cmd.front() == '/')
                cmd = cmd.substr(1, cmd.size() - 1);

            CallEvent(EVENT_TYPES::OnPlayerCmd, NewPlayer(player), cmd);
        }
    }
    return original(_this, id, pkt);
}

// ===== onUseItem =====
THook(bool, "?useItemOn@GameMode@@UEAA_NAEAVItemStack@@AEBVBlockPos@@EAEBVVec3@@PEBVBlock@@@Z",
    void* _this,  ItemStack* item, BlockPos* bp, unsigned __int8 a4, Vec3* a5,  Block* bl)
{
    IF_EXIST(EVENT_TYPES::OnUseItem)
    {
        auto sp = *reinterpret_cast<Player**>(reinterpret_cast<unsigned long long>(_this) + 8);

        CallEventEx(EVENT_TYPES::OnUseItem, NewPlayer(sp), NewBlock(bl), NewPos(bp->x, bp->y, bp->z));
    }
    return original(_this, item, bp, a4, a5, bl);
}

// ===== onDestroyBlock =====
THook(bool, "?checkBlockDestroyPermissions@BlockSource@@QEAA_NAEAVActor@@AEBVBlockPos@@AEBVItemStackBase@@_N@Z",
    BlockSource * _this, Actor* player, BlockPos* pos, ItemStack* a3, bool a4)
{
    IF_EXIST(EVENT_TYPES::OnDestroyBlock)
    {
        auto block = SymCall("?getBlock@BlockSource@@QEBAAEBVBlock@@AEBVBlockPos@@@Z", Block*, BlockSource*, BlockPos*)(_this, pos);
///////////////////////////// player? /////////////////////////////
        CallEventEx(EVENT_TYPES::OnDestroyBlock, NewEntity(player), NewBlock(block), NewPos(pos->x, pos->y, pos->z));
    }
    return original(_this, player, pos,a3, a4);
}

// ===== onPlaceBlock =====
THook(bool, "?mayPlace@BlockSource@@QEAA_NAEBVBlock@@AEBVBlockPos@@EPEAVActor@@_N@Z",
    BlockSource* bs, Block* bl, BlockPos* a3, unsigned __int8 a4, Actor* pl, bool a6)
{
    IF_EXIST(EVENT_TYPES::OnPlaceBlock)
    {
/////////////////////////////////// Player? ////////////////////////////////////////
        CallEventEx(EVENT_TYPES::OnPlaceBlock, NewEntity(pl), NewBlock(bl), NewPos(a3->x, a3->y, a3->z));
    }
    return original(bs, bl, a3, a4, pl, a6);
}

// ===== onOpenChest =====
THook(bool, "?use@ChestBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    void* _this, Player* pl , BlockPos* bp)
{
    IF_EXIST(EVENT_TYPES::OnOpenChest)
    {
        CallEventEx(EVENT_TYPES::OnOpenChest, NewPlayer(pl), NewPos(bp->x, bp->y, bp->z));
    }
    return original(_this, pl, bp);
}

// ===== onCloseChest =====
THook(bool, "?stopOpen@ChestBlockActor@@UEAAXAEAVPlayer@@@Z",
    void* _this, Player* pl)
{
    IF_EXIST(EVENT_TYPES::OnCloseChest)
    {
        auto bp = (BlockPos*)((intptr_t*)_this - 204);

        CallEventEx(EVENT_TYPES::OnCloseChest, NewPlayer(pl), NewPos(bp->x, bp->y, bp->z));
    }
    return original(_this, pl);
}

// ===== onOpenBarrel =====
THook(bool, "?use@BarrelBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    void* _this, Player* pl, BlockPos* bp)
{
    IF_EXIST(EVENT_TYPES::OnOpenBarrel)
    {
        CallEventEx(EVENT_TYPES::OnOpenBarrel, NewPlayer(pl), NewPos(bp->x, bp->y, bp->z));
    }
    return original(_this, pl, bp);
}

// ===== onCloseBarrel =====
THook(bool, "?stopOpen@BarrelBlockActor@@UEAAXAEAVPlayer@@@Z",
    void* _this, Player* pl)
{
    IF_EXIST(EVENT_TYPES::OnCloseBarrel)
    {
        auto bp = (BlockPos*)((intptr_t*)_this - 204);

        CallEventEx(EVENT_TYPES::OnCloseBarrel, NewPlayer(pl), NewPos(bp->x, bp->y, bp->z));
    }
    return original(_this, pl);
}

// ===== onChangeSlot =====
THook(void, "?containerContentChanged@LevelContainerModel@@UEAAXH@Z",
    LevelContainerModel* a1, int a2)
{
    IF_EXIST(EVENT_TYPES::OnChangeSlot)
    {
        Actor* v3 = dAccess<Actor*, 208>(a1);
        BlockSource* bs = dAccess<BlockSource*, 840>(v3);
        //offset LevelContainerModel::_getContainer(LevelContainerModel *this) Line->BlockSource::getBlockEntity
        BlockPos* bpos = offLevelContainerModel::getBlockPos(a1);
        Block* pBlk = SymBlockSource::getBlock(bs, bpos);
        auto v5 = (*(__int64(__fastcall**)(LevelContainerModel*))(*(uintptr_t*)a1 + 160i64))(a1);
        if (v5)
        {
            ItemStack* item = (ItemStack*)(*(__int64(__fastcall**)(__int64, uintptr_t))(*(uintptr_t*)v5 + 40i64))(v5, a2);

            int count = offItemStack::getCount(item);
            std::string name = offBlock::getFullName(pBlk);
            //去掉前缀
            if (name.find("minecraft:") != string::npos)
                name = name.replace(name.find("minecraft:"), 10, "");
            std::string itemName = item->getName();
            int slotNumber = a2;
            Actor* pl = v3;
///////////////////////////// ?pl ///////////////////////////// 
            CallEvent(EVENT_TYPES::OnChangeSlot, NewEntity(pl), NewBlock(pBlk), slotNumber, NewPos(bpos->x, bpos->y, bpos->z), count != 0, NewItem(item));
        }
    }
	//return original(a1, a2);
////////////////////////////////////// 返回值？ /////////////////////////////////
}

// ===== onMobDie =====
THook(bool, "?die@Mob@@UEAAXAEBVActorDamageSource@@@Z",
    Mob* self, void* ads)
{
    IF_EXIST(EVENT_TYPES::OnMobDie)
    {
        char v83;
        auto v6 = *(void**)(*(__int64(__fastcall**)(void*, char*))(*(intptr_t*)ads + 64i64))(ads, &v83);
        auto level = offPlayer::getLevel(self);
        auto ac = SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z"
            , Actor*, Level*, void*, bool)(level, v6, 0);
/////////////////////////////  ADD MOB ///////////////////////////// 
        CallEventEx(EVENT_TYPES::OnMobDie, NewEntity(self), NewEntity(ac));
    }
    return original(self, ads);
}

// ===== onMobHurt =====
THook(bool, "?_hurt@Mob@@MEAA_NAEBVActorDamageSource@@H_N1@Z",
    Mob* self, void* ads, int a1, bool a2, bool a3)
{
    IF_EXIST(EVENT_TYPES::OnMobHurt)
    {
        char v83;
        auto v6 = *(void**)(*(__int64(__fastcall**)(void*, char*))(*(intptr_t*)ads + 64i64))(ads, &v83);
        auto level = offPlayer::getLevel(self);
        auto ac = SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z"
            , Actor*, Level*, void*, bool)(level, v6, 0);
        
        CallEventEx(EVENT_TYPES::OnMobHurt, NewEntity(self), NewEntity(ac));
    }
    return original(self, ads, a1, a2, a3);
}

// ===== onExplode =====
THook(bool, "?explode@Level@@UEAAXAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
    Level* _this, BlockSource* bs, Actor* actor, Vec3 pos, float a5, bool a6, bool a7, float a8, bool a9)
{
    IF_EXIST(EVENT_TYPES::OnExplode)
    {
        CallEventEx(EVENT_TYPES::OnExplode, NewEntity(actor), NewPos(pos.x, pos.y, pos.z));
    }
    return original(_this, bs, actor, pos, a5, a6, a7, a8, a9);
}

// ===== onServerCmd =====
THook(bool, "?executeCommand@MinecraftCommands@@QEBA?AUMCRESULT@@V?$shared_ptr@VCommandContext@@@std@@_N@Z",
    MinecraftCommands* _this, unsigned int* a2, std::shared_ptr<CommandContext> x, char a4)
{
    Player* player = MakeSP(x->getOrigin());
    if(!player)
    {   // Server Command
        string cmd = x->getCmd();
        if (cmd.front() == '/')
            cmd = cmd.substr(1, cmd.size() - 1);
        
        //后台调试
        extern bool globalDebug;
        extern std::shared_ptr<ScriptEngine> debugEngine;

        if(cmd == LXL_DEBUG_CMD)
        {
            if(globalDebug)
            {   //EndDebug
                INFO("Debug mode ended");
                globalDebug = false;
                return false;
            }
            else
            {   //StartDebug
                INFO("Debug mode begin");
                globalDebug = true;
                cout << LXL_SCRIPT_LANG_TYPE << ">" << flush;
                return false;
            }
        }
        if(globalDebug)
        {
            EngineScope enter(debugEngine.get());
            try
            {
                auto result = debugEngine->eval(cmd);
                PrintValue(cout,result);
                cout << "\n" << LXL_SCRIPT_LANG_TYPE << ">" << flush;
            }
            catch(Exception& e)
            {
                ERRPRINT(e);
                cout << LXL_SCRIPT_LANG_TYPE << ">" << flush;
            }
            return false;
        }

        CallEventEx(EVENT_TYPES::OnServerCmd, cmd);
    }
    return original(_this, a2, x, a4);
}