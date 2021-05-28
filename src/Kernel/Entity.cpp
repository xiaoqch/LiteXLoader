#include "Entity.h"
using namespace std;

string Raw_GetEntityName(Actor* actor)
{
    return actor->getNameTag();
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