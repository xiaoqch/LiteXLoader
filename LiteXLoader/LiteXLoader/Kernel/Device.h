#pragma once
#include <string>

class Player;
std::string Raw_GetIP(Player* player);
int Raw_GetAvgPing(Player* player);
float Raw_GetAvgPacketloss(Player* player);
std::string Raw_GetOs(Player* player);
std::string Raw_GetClientId(Player* player);

std::string Raw_GetOsNameByType(int type);