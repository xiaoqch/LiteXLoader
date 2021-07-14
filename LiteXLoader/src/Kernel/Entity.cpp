#include "Entity.h"
using namespace std;

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

int Raw_GetEntityId(Actor* actor)
{
    return actor->getEntityTypeId();
}

FloatVec4 Raw_GetEntityPos(Actor* actor)
{
    auto pos = actor->getPos();
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

Player* Raw_ToPlayer(Actor* actor)
{
    if (Raw_GetEntityTypeName(actor) == "entity.player.name")
        return dAccess<Player*>(actor, 0);
    else
        return nullptr;
}