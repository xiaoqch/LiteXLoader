#include "Global.h"
#include "Entity.h"
#include "Player.h"
#include "SymbolHelper.h"
#include <string>
#include <vector>
using namespace std;

class Spawner;
Actor* Raw_SpawnMob(std::string name, const FloatVec4& pos)
{
    try
    {
        if (name.find("minecraft:") == 0)
            name = name.substr(10);

        char a[168];
        ActorDefinitionIdentifier* ad = SymCall("??0ActorDefinitionIdentifier@@QEAA@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
            ActorDefinitionIdentifier*, ActorDefinitionIdentifier*, string&)((ActorDefinitionIdentifier*)a, name);

        Spawner* sp = SymCall("?getSpawner@Level@@UEBAAEAVSpawner@@XZ", Spawner*, Level*)(mc->getLevel());

        Vec3 vec{ pos.x,pos.y,pos.z };
        Actor* ac = SymCall("?spawnMob@Spawner@@QEAAPEAVMob@@AEAVBlockSource@@AEBUActorDefinitionIdentifier@@PEAVActor@@AEBVVec3@@_N44@Z",
            Mob*, Spawner * _this, BlockSource*, ActorDefinitionIdentifier*, Actor*, Vec3*, bool, bool, bool)
            (sp, Raw_GetBlockSourceByDim(pos.dim), ad, nullptr, &vec, 0, 1, 0);

        return ac;
    }
    catch(...)
    {
        return nullptr;
    }
}

string Raw_GetEntityName(Actor* actor)
{
    string name = actor->getNameTag();
    return name.empty() ? Raw_GetEntityTypeName(actor) : name;
}

std::string Raw_GetEntityTypeName(Actor* actor)
{
    return SymCall("?EntityTypeToString@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4ActorType@@W4ActorTypeNamespaceRules@@@Z",
        string, int, int)
        (actor->getEntityTypeId(), 1);
}

int Raw_GetEntityTypeId(Actor* actor)
{
    return actor->getEntityTypeId();
}

FloatVec4 Raw_GetEntityPos(Actor* actor)
{
    Vec3 pos;
    SymCall("?getFeetPos@CommandUtils@@YA?AVVec3@@PEBVActor@@@Z",
        Vec3*, Vec3*, Actor*)(&pos, actor);

    return {pos.x,pos.y,pos.z,WActor(*actor).getDimID()};
}

int Raw_GetHealth(Actor *actor)
{
    return SymCall("?getHealth@Actor@@QEBAHXZ", int, Actor*)(actor);
}

int Raw_GetMaxHealth(Actor *actor)
{
    return SymCall("?getMaxHealth@Actor@@QEBAHXZ", int, Actor*)(actor);
}

bool Raw_GetIsInAir(Actor *actor)
{
    return !(dAccess<bool,448>(actor));
}

bool Raw_TeleportEntity(Actor* actor, const FloatVec4 &pos)
{
    WActor(*actor).teleport({pos.x,pos.y,pos.z},pos.dim);
    return true;
}

bool Raw_KillEntity(Actor* actor)
{
    SymCall("?kill@Mob@@UEAAXXZ", void, void*)(actor);
    return true;
}

bool Raw_IsPlayer(Actor* actor)
{
    Player* pl = (Player*)actor;
    auto plList = Raw_GetOnlinePlayers();

    for (auto& player : plList)
        if (player == pl)
        {
            return true;
        }
    return false;
}

Player* Raw_ToPlayer(Actor* actor)
{
    return Raw_IsPlayer(actor) ? (Player*)actor : nullptr;
}

int Raw_GetEntityDimId(Actor* actor)
{
    return WActor(*actor).getDimID();
}


bool Raw_SetOnFire(Actor* actor, int time) {
    SymCall("?setOnFire@Actor@@UEAAXH@Z", void, 
        Actor*, int)(actor, time);
    return true;
}

/*
bool Raw_SetInLove(Actor* a1, Actor* a2) {
    SymCall("?setInLove@Actor@@QEAAXPEAV1@@Z", void,
        Actor*, Actor*)(a1, a2);
    return true;
}*/

Actor* Raw_GetEntityByUniqueId(ActorUniqueID id) {
    return SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z",
        Actor*, Level*, ActorUniqueID, bool)(mc->getLevel(), id, 0);
}

float Raw_GetSpeed(Actor* ac) {
    return SymCall("?getSpeedInMetersPerSecond@Actor@@QEBAMXZ"
        , float, Actor*)(ac);
}

bool Raw_AddTag(Actor* ac, const string& str) {
    return SymCall("?addTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
        bool, Actor*, const string*)(ac, &str);
}

bool Raw_RemoveTag(Actor* ac, const string& str) {
    return SymCall("?removeTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
        bool, Actor*, const string*)(ac, &str);
}