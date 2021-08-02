#include <string>
#include <vector>

std::string Raw_GetPlayerName(Player* player);
FloatVec4 Raw_GetPlayerPos(Player* player);
std::string Raw_GetXuid(Player* player);
std::string Raw_GetUuid(Player *player);
std::string Raw_GetRealName(Player* player);
std::string Raw_GetIP(Player* player);
int Raw_GetPlayerPermLevel(Player* player);
int Raw_GetGameMode(Player* player);
bool Raw_GetSneaking(Player *player);
int Raw_GetDirection(Player* player);

bool Raw_RuncmdAs(Player *player, const std::string &cmd);
bool Raw_TeleportPlayer(Player* player, const FloatVec4 &pos);
bool Raw_KillPlayer(Player* player);
bool Raw_SetPlayerPermLevel(Player* player, int permLevel);
bool Raw_SetGameMode(Player* player, int gameMode);
bool Raw_KickPlayer(Player* player, const std::string &msg);
bool Raw_Tell(Player* player, const std::string &text, TextType type = TextType::RAW);
bool Raw_Broadcast(const std::string& text, TextType type = TextType::RAW);
ItemStack* Raw_GetHand(Player* player);
bool Raw_GetAllItems(Player* player, ItemStack** hand, ItemStack** offHand, std::vector<ItemStack*>* inventory,
    std::vector<ItemStack*>* armor, std::vector<ItemStack*>* endChest);
bool Raw_RenamePlayer(Player* player, const std::string &name);
bool Raw_AddLevel(Player* player, int level);
bool Raw_TransServer(Player* player, const std::string& server, short port);
bool Raw_CrashPlayer(Player* player);

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
Player* Raw_GetPlayerByUniqueId(ActorUniqueID id);
bool Raw_RefreshInventory(Player* pl);
bool Raw_RemoveItem(Player* pl, int inventoryId, int count);