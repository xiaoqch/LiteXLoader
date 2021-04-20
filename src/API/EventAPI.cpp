#include "EventAPI.h"
#include <vector>
#include <map>
#include <exception>
#include "APIhelp.h"
using namespace script;

enum class EVENT_TYPES : int
{
	OnJoin=0, OnLeft, OnRespawn, OnChangeDim,
	OnCmd, ONLCMD, OnChat,
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
	{"onPlayerCommand",EVENT_TYPES::OnCmd},
    {"onServerCommand",EVENT_TYPES::ONLCMD},
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
static std::vector<Local<Function>> listenerList[EVENT_TYPES::EVENT_COUNT];

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
        listenerList[eventId].push_back(args[1].asFunction());
        return Boolean::newBoolean(true);
    }
    catch(const std::logic_error& e)
    {
        DEBUG("Event \""+ args[0].asString().toString() +"\" No Found!");
        return Boolean::newBoolean(false);
    }
    
}