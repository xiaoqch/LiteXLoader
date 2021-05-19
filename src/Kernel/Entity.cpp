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