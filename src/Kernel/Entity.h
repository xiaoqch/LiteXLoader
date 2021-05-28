#include "Global.h"
#include <string>

std::string Raw_GetEntityName(Actor* actor);
FloatVec4 Raw_GetEntityPos(Actor* actor);
int Raw_GetHealth(Actor *actor);
int Raw_GetMaxHealth(Actor *actor);

bool Raw_TeleportEntity(Actor* actor, const FloatVec4 &pos);
bool Raw_KillEntity(Actor* actor);