#pragma once
#include <string>

std::string Raw_GetEntityName(Actor* actor);
std::string Raw_GetEntityTypeName(Actor* actor);
int Raw_GetEntityTypeId(Actor* actor);
FloatVec4 Raw_GetEntityPos(Actor* actor);
int Raw_GetHealth(Actor *actor);
int Raw_GetMaxHealth(Actor *actor);
bool Raw_GetIsInAir(Actor *actor);

bool Raw_TeleportEntity(Actor* actor, const FloatVec4 &pos);
bool Raw_KillEntity(Actor* actor);
bool Raw_SetOnFire(Actor* actor,int time);
bool Raw_IsPlayer(Actor* actor);
Player* Raw_ToPlayer(Actor* actor);

int Raw_GetEntityDimId(Actor* actor);
Actor* Raw_GetEntityByUniqueId(ActorUniqueID id);
float Raw_GetSpeed(Actor* ac);