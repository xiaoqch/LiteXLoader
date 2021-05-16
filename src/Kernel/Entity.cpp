#include "Entity.h"
using namespace std;

string inline Raw_GetEntityName(Actor* actor)
{
    return actor->getNameTag();
}

FloatVec4 Raw_GetEntityPos(Actor* actor)
{
    auto pos = actor->getPos();
    return {pos.x,pos.y,pos.z,WActor(*actor).getDimID()};
}

bool inline Raw_TeleportEntity(Actor* actor, const FloatVec4 &pos)
{
    WActor(*actor).teleport({pos.x,pos.y,pos.z},pos.dim);
    return true;
}

bool inline Raw_KillEntity(Actor* actor)
{
    SymCall("?kill@Mob@@UEAAXXZ", void, void*)(actor);
    return true;
}