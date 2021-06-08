#include "Global.h"
#include <string>
#include <vector>

std::string Raw_GetPlayerName(Player* player);
FloatVec4 Raw_GetPlayerPos(Player* player);
std::string Raw_GetXuid(Player* player);
std::string Raw_GetUuid(Player *player);
std::string Raw_GetRealName(Player* player);
std::string Raw_GetIP(Player* player);
bool Raw_GetSneaking(Player *player);

bool Raw_RuncmdAs(Player *player, const std::string &cmd);
bool Raw_TeleportPlayer(Player* player, const FloatVec4 &pos);
bool Raw_KillPlayer(Player* player);
int Raw_GetPlayerPermLevel(Player* player);
bool Raw_SetPlayerPermLevel(Player* player, int permLevel);
bool Raw_KickPlayer(Player* player, const std::string &msg);
bool Raw_Tell(Player* player, const std::string &text, TextType type = TextType::RAW);
bool Raw_Broadcast(const std::string& text, TextType type = TextType::RAW);
ItemStack* Raw_GetHand(Player* player);
ItemStack* Raw_GetOffHand(Player* player);
std::vector<ItemStack*> Raw_GetPack(Player* player);
bool Raw_RenamePlayer(Player* player, const std::string &name);
bool Raw_AddLevel(Player* player, int level);
bool Raw_TransServer(Player* player, const std::string& server, short port);

int Raw_GetScore(Player* player, const std::string &key);
bool Raw_SetScore(Player* player, const std::string &key, int value);
bool Raw_AddScore(Player* player, const std::string &key, int value);
bool Raw_RemoveScore(Player* player, const std::string &key);
bool Raw_SetSidebar(Player *player, std::string title, const std::vector<std::pair<std::string,int>> &data);
bool Raw_RemoveSidebar(Player *player);
bool Raw_SetBossBar(Player *player, std::string title, float percent);
bool Raw_RemoveBossBar(Player *player);

std::vector<Player*> Raw_GetOnlinePlayers();
bool Raw_IsPlayerValid(Player *player);

int Raw_GetPlayerDimId(Player* player);