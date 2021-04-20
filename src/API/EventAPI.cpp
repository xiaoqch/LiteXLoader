#include "EventAPI.h"
#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include "APIhelp.h"
#include "../LiteLoader/headers/api/commands.h"
using namespace script;


enum class EVENT_TYPES : int
{
	OnJoin=0, OnLeft, OnRespawn, OnChangeDim,
	OnCmd, OnChat,
    OnAttack, OnMobDeath, OnMobHurt,
    OnUseItem, OnTakeItem, OnDropItem,
	OnDestroyBlock, OnPlaceBlock, OnExplode,
	OnOpenChest, OnCloseChest, OnOpenBarrel, OnCloseBarrel,
	EVENT_COUNT
};
static const std::unordered_map<string, EVENT_TYPES> EventsMap{
    {"onPlayerJoin",EVENT_TYPES::OnJoin},
    {"onPlayerLeft",EVENT_TYPES::OnLeft},
    {"onRespawn",EVENT_TYPES::OnRespawn},
    {"onChangeDimension",EVENT_TYPES::OnChangeDim},
	{"onPlayerCmd",EVENT_TYPES::OnCmd},
    {"onChat",EVENT_TYPES::OnChat},
    {"onAttack",EVENT_TYPES::OnAttack},
    {"onMobDeath",EVENT_TYPES::OnMobDeath},
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
    for(int i=0;i<listenerList[int(type)].size();++i)
    {
        ListenerListType *nowListener = &(listenerList[int(type)][i]);
        EngineScope enter(nowListener->engine);
        try{
            auto result = nowListener->func.get().call({},args);
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

    try
    {
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
        DEBUG("Event \""+ args[0].asString().toString() +"\" No Found!");
        return Boolean::newBoolean(false);
    }   
}



//////////////////// Hook ////////////////////

//随便进一个一定存在的EngineScope，否则构造参数时会崩
#define EngineBegin(e) \
    if(!listenerList[int(e)].empty()){ \
        EngineScope enter(listenerList[int(e)][0].engine)
#define EngineEnd() }


// ===== ServerCmd =====
THook(bool, "?executeCommand@MinecraftCommands@@QEBA?AUMCRESULT@@V?$shared_ptr@VCommandContext@@@std@@_N@Z",
	MinecraftCommands* _this, unsigned int* a2, std::shared_ptr<CommandContext> x, char a4)
{
	Player* player = MakeSP(x->getOrigin());

	string cmd = x->getCmd();
	if (cmd.at(0) == '/')
		cmd = cmd.substr(1, cmd.size() - 1);

    EngineBegin(EVENT_TYPES::OnCmd);
    if(!CallEvent(EVENT_TYPES::OnCmd,{Number::newNumber((int64_t)(intptr_t)player),String::newString(cmd)}))
         return false;
    EngineEnd();
	return original(_this, a2, x, a4);
}