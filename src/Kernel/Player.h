#include "Global.h"
#include <string>
#include <vector>

std::string Raw_GetPlayerName(Player* player);
FloatVec4 Raw_GetPlayerPos(Player* player);
std::string Raw_GetXuid(Player* player);
std::string Raw_GetRealName(Player* player);
std::string Raw_GetIP(Player* player);

bool Raw_RuncmdAs(Player *player, const std::string &cmd);
bool Raw_TeleportPlayer(Player* player, const FloatVec4 &pos);
bool Raw_KillPlayer(Player* player);
int Raw_GetPlayerPermLevel(Player* player);
bool Raw_SetPlayerPermLevel(Player* player, int permLevel);
bool Raw_KickPlayer(Player* player, const std::string &msg);
bool Raw_Tell(Player* player, const std::string &text, TextType type);
ItemStack* Raw_GetHand(Player* player);
bool Raw_RenamePlayer(Player* player, const std::string &name);

std::vector<Player*> Raw_GetOnlinePlayers();