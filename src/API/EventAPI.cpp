#include "EventAPI.h"
#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include "APIhelp.h"
#include "../LiteLoader/headers/api/commands.h"
#include "../LiteLoader/headers/mc/OffsetHelper.h"
using namespace script;

//////////////////// Definitions ////////////////////

enum class EVENT_TYPES : int
{
    OnJoin=0, OnLeft, OnRespawn, OnChangeDim,
    OnPlayerCmd, OnChat, OnAttack,
    OnUseItem, OnTakeItem, OnDropItem,
    OnDestroyBlock, OnPlaceBlock,
    OnOpenChest, OnCloseChest, OnOpenBarrel, OnCloseBarrel,
    OnMobDie, OnMobHurt, OnExplode, OnCmdBlockExecute,
    OnServerStarted,OnServerCmd,
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
bool CallEvent(EVENT_TYPES type,std::initializer_list<Local<Value>> args)
{
    std::vector<ListenerListType> &nowList = listenerList[int(type)];

    for(int i = 0; i < nowList.size(); ++i)
    {
        EngineScope enter(nowList[i].engine);
        try{
            auto result = nowList[i].func.get().call({},args);
            if(result.isBoolean() && result.asBoolean().value() == false)
                return false;
        }
        catch(const script::Exception& e)
        {
            ERROR("Event Callback Failed!");
            std::cerr << e.message() << std::endl;
            return true;
        }
    }
    return true;
}



//////////////////// APIs ////////////////////

script::Local<script::Value> AddEventListener(const script::Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kFunction)

    try{
        int eventId = int(EventsMap.at(args[0].asString().toString()));
        /*switch(eventId)
        {
            break;
        }*/
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



//////////////////// Hook ////////////////////

//随便进入一个已经存在的EngineScope，否则CallEvent构造参数时会崩
#define EngineBegin(e) \
    if(!listenerList[int(e)].empty()){ \
        EngineScope enter(listenerList[int(e)][0].engine)
#define EngineEnd() }


void InitEventAPIs()
{
// ===== onPlayerJoin =====
    Event::addEventListener([](JoinEV ev)
    {
        EngineBegin(EVENT_TYPES::OnJoin);
        CallEvent(EVENT_TYPES::OnJoin,{ Number::newNumber((intptr_t)ev.Player) });
        EngineEnd();
    });

// ===== onPlayerLeft =====
    Event::addEventListener([](LeftEV ev)
    {
        EngineBegin(EVENT_TYPES::OnLeft);
        CallEvent(EVENT_TYPES::OnLeft,{ Number::newNumber((intptr_t)ev.Player) });
        EngineEnd();
    });

// ===== onChat =====
    Event::addEventListener([](ChatEV ev)
    {
        EngineBegin(EVENT_TYPES::OnChat);
        if (!CallEvent(EVENT_TYPES::OnChat,{ Number::newNumber((intptr_t)ev.pl), String::newString(ev.msg) }))
            return false;
        EngineEnd();
        return true;
    });

// ===== onChangeDimension =====
    Event::addEventListener([](ChangeDimEV ev)
    {
        EngineBegin(EVENT_TYPES::OnChangeDim);
        CallEvent(EVENT_TYPES::OnChangeDim,{ Number::newNumber((intptr_t)ev.Player) });
        EngineEnd();
    });

// ===== onCmdBlockExecute =====
    Event::addEventListener([](CmdBlockExeEV ev)
    {
        EngineBegin(EVENT_TYPES::OnCmdBlockExecute);
        if (!CallEvent(EVENT_TYPES::OnCmdBlockExecute,{ String::newString(ev.cmd), Number::newNumber(ev.bpos.x),
            Number::newNumber(ev.bpos.y), Number::newNumber(ev.bpos.z) }))
                return false;
        EngineEnd();
        return true;
    });

// ===== onServerStarted =====
    Event::addEventListener([](ServerStartedEV ev)
    {
        EngineBegin(EVENT_TYPES::OnServerStarted);
        CallEvent(EVENT_TYPES::OnServerStarted,{});
        EngineEnd();
    });

}

// ===== onAttack =====
THook(bool, "?attack@Player@@UEAA_NAEAVActor@@@Z",
    Player* pl,  Actor* ac)
{
    EngineBegin(EVENT_TYPES::OnAttack);
    if (!CallEvent(EVENT_TYPES::OnAttack,{ Number::newNumber((intptr_t)pl), Number::newNumber((intptr_t)ac) }))
        return false;
    EngineEnd();
    return original(pl, ac);
}

// ===== onRespawn =====
THook(bool, "?respawn@Player@@UEAAXXZ",
    Player* pl)
{
    EngineBegin(EVENT_TYPES::OnRespawn);
    if (!CallEvent(EVENT_TYPES::OnRespawn,{ Number::newNumber((intptr_t)pl) }))
        return false;
    EngineEnd();
    return original(pl);
}

// ===== onDropItem =====
THook(bool, "?drop@Player@@UEAA_NAEBVItemStack@@_N@Z",
    Player* _this, ItemStack* a2, bool a3)
{
    EngineBegin(EVENT_TYPES::OnDropItem);
    if (!CallEvent(EVENT_TYPES::OnDropItem,{ Number::newNumber((intptr_t)_this), Number::newNumber((intptr_t)a2) }))
        return false;
    EngineEnd();
    return original(_this, a2, a3);
}

// ===== onTakeItem =====
THook(bool, "?take@Player@@QEAA_NAEAVActor@@HH@Z",
    Player* _this, Actor* actor, int a2, int a3)
{
    EngineBegin(EVENT_TYPES::OnTakeItem);
    if (!CallEvent(EVENT_TYPES::OnTakeItem,{ Number::newNumber((intptr_t)_this), Number::newNumber((intptr_t)actor) }))
        return false;
    EngineEnd();
    return original(_this, actor, a2, a3);
}

// ===== onPlayerCmd =====
THook(void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
    ServerNetworkHandler* _this, NetworkIdentifier* id, void* pkt)
{
    auto player = SymCall("?_getServerPlayer@ServerNetworkHandler@@AEAAPEAVServerPlayer@@AEBVNetworkIdentifier@@E@Z",
        Player*, void*, void*, char)(_this, id, *(char*)((uintptr_t)pkt + 16));
    if (player)
    {   // Player Command
        auto cmd = std::string(*(std::string*)((uintptr_t)pkt + 48));
        if (cmd.front() == '/')
            cmd = cmd.substr(1, cmd.size() - 1);

        EngineBegin(EVENT_TYPES::OnPlayerCmd);
        if(!CallEvent(EVENT_TYPES::OnPlayerCmd,{ Number::newNumber((intptr_t)player),String::newString(cmd) }))
            return;
        EngineEnd();
    }
    return original(_this, id, pkt);
}

// ===== onUseItem =====
THook(bool, "?useItemOn@GameMode@@UEAA_NAEAVItemStack@@AEBVBlockPos@@EAEBVVec3@@PEBVBlock@@@Z",
    void* _this,  ItemStack* item, BlockPos* bp, unsigned __int8 a4, Vec3* a5,  Block* bl)
{
    auto sp = *reinterpret_cast<Player**>(reinterpret_cast<unsigned long long>(_this) + 8);

    EngineBegin(EVENT_TYPES::OnUseItem);
    if (!CallEvent(EVENT_TYPES::OnUseItem,{ Number::newNumber((intptr_t)sp), Number::newNumber((intptr_t)bl), 
        Number::newNumber(bp->x), Number::newNumber(bp->y), Number::newNumber(bp->z) }))
            return false;
    EngineEnd();
    return original(_this, item, bp, a4, a5, bl);
}

// ===== onDestroyBlock =====
THook(bool, "?checkBlockDestroyPermissions@BlockSource@@QEAA_NAEAVActor@@AEBVBlockPos@@AEBVItemStackBase@@_N@Z",
    BlockSource * _this, Actor* player, BlockPos* pos, ItemStack* a3, bool a4)
{
    auto block = SymCall("?getBlock@BlockSource@@QEBAAEBVBlock@@AEBVBlockPos@@@Z", Block*, BlockSource*, BlockPos*)(_this, pos);

    EngineBegin(EVENT_TYPES::OnDestroyBlock);
    if (!CallEvent(EVENT_TYPES::OnDestroyBlock,{ Number::newNumber((intptr_t)player),
        Number::newNumber((intptr_t)block), Number::newNumber(pos->x),
        Number::newNumber(pos->y), Number::newNumber(pos->z) }))
            return false;
    EngineEnd();
    return original(_this, player, pos,a3, a4);
}

// ===== onPlaceBlock =====
THook(bool, "?mayPlace@BlockSource@@QEAA_NAEBVBlock@@AEBVBlockPos@@EPEAVActor@@_N@Z",
    BlockSource* bs, Block* bl, BlockPos* a3, unsigned __int8 a4, Actor* pl, bool a6)
{
    EngineBegin(EVENT_TYPES::OnPlaceBlock);
    if (!CallEvent(EVENT_TYPES::OnPlaceBlock,{ Number::newNumber((intptr_t)pl), Number::newNumber((intptr_t)bl),
        Number::newNumber(a3->x), Number::newNumber(a3->y), Number::newNumber(a3->z) }))
            return false;
    EngineEnd();
    return original(bs, bl, a3, a4, pl, a6);
}

// ===== onOpenChest =====
THook(bool, "?use@ChestBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    void* _this, Player* pl , BlockPos* bp)
{
    EngineBegin(EVENT_TYPES::OnOpenChest);
    if (!CallEvent(EVENT_TYPES::OnOpenChest,{ Number::newNumber((intptr_t)pl),Number::newNumber(bp->x),
        Number::newNumber(bp->y), Number::newNumber(bp->z) }))
            return false;
    EngineEnd();
    return original(_this, pl, bp);
}

// ===== onCloseChest =====
THook(bool, "?stopOpen@ChestBlockActor@@UEAAXAEAVPlayer@@@Z",
    void* _this, Player* pl)
{
    auto bp = (BlockPos*)((intptr_t*)_this - 204);

    EngineBegin(EVENT_TYPES::OnCloseChest);
    if (!CallEvent(EVENT_TYPES::OnCloseChest,{ Number::newNumber((intptr_t)pl),Number::newNumber(bp->x),
        Number::newNumber(bp->y), Number::newNumber(bp->z) }))
            return false;
    EngineEnd();
    return original(_this, pl);
}

// ===== onOpenBarrel =====
THook(bool, "?use@BarrelBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    void* _this, Player* pl, BlockPos* bp)
{
    EngineBegin(EVENT_TYPES::OnOpenBarrel);
    if (!CallEvent(EVENT_TYPES::OnOpenBarrel,{ Number::newNumber((intptr_t)pl),Number::newNumber(bp->x),
        Number::newNumber(bp->y), Number::newNumber(bp->z) }))
            return false;
    EngineEnd();
    return original(_this, pl, bp);
}

// ===== onCloseBarrel =====
THook(bool, "?stopOpen@BarrelBlockActor@@UEAAXAEAVPlayer@@@Z",
    void* _this, Player* pl)
{
    auto bp = (BlockPos*)((intptr_t*)_this - 204);

    EngineBegin(EVENT_TYPES::OnCloseBarrel);
    if (!CallEvent(EVENT_TYPES::OnCloseBarrel,{ Number::newNumber((intptr_t)pl), Number::newNumber(bp->x),
        Number::newNumber(bp->y), Number::newNumber(bp->z) }))
            return false;
    EngineEnd();
    return original(_this, pl);
}

// ===== onMobDie =====
THook(bool, "?die@Mob@@UEAAXAEBVActorDamageSource@@@Z",
    Mob* self, void* ads)
{
    char v83;
    auto v6 = *(void**)(*(__int64(__fastcall**)(void*, char*))(*(intptr_t*)ads + 64i64))(ads, &v83);
    auto level = offPlayer::getLevel(self);
    auto ac = SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z"
        , Actor*, Level*, void*, bool)(level, v6, 0);
    
    EngineBegin(EVENT_TYPES::OnMobDie);
    if (!CallEvent(EVENT_TYPES::OnMobDie,{ Number::newNumber((intptr_t)self), Number::newNumber((intptr_t)ac) }))
        return false;
    EngineEnd();
    return original(self, ads);
}

// ===== onMobHurt =====
THook(bool, "?_hurt@Mob@@MEAA_NAEBVActorDamageSource@@H_N1@Z",
    Mob* self, void* ads, int a1, bool a2, bool a3)
{
    char v83;
    auto v6 = *(void**)(*(__int64(__fastcall**)(void*, char*))(*(intptr_t*)ads + 64i64))(ads, &v83);
    auto level = offPlayer::getLevel(self);
    auto ac = SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z"
        , Actor*, Level*, void*, bool)(level, v6, 0);
    
    EngineBegin(EVENT_TYPES::OnMobHurt);
    if (!CallEvent(EVENT_TYPES::OnMobHurt,{ Number::newNumber((intptr_t)self), Number::newNumber((intptr_t)ac) }))
        return false;
    EngineEnd();
    return original(self, ads, a1, a2, a3);
}

// ===== onExplode =====
THook(bool, "?explode@Level@@UEAAXAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
    Level* _this, BlockSource* bs, Actor* actor, Vec3 pos, float a5, bool a6, bool a7, float a8, bool a9)
{
    EngineBegin(EVENT_TYPES::OnExplode);
    if (!CallEvent(EVENT_TYPES::OnExplode,{ Number::newNumber((intptr_t)actor),Number::newNumber(pos.x),
        Number::newNumber(pos.y),Number::newNumber(pos.z) }))
            return false;
    EngineEnd();
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

        EngineBegin(EVENT_TYPES::OnServerCmd);
        if(!CallEvent(EVENT_TYPES::OnServerCmd,{ String::newString(cmd) }))
            return false;
        EngineEnd();
    }
    return original(_this, a2, x, a4);
}