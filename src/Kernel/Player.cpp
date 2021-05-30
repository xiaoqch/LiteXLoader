#include "Player.h"
using namespace std;

string  Raw_GetPlayerName(Player* player)
{
    return player->getNameTag();
}

FloatVec4 Raw_GetPlayerPos(Player* player)
{
    auto pos = player->getPos();
    return {pos.x,pos.y,pos.z,WPlayer(*player).getDimID()};
}

string  Raw_GetXuid(Player* player)
{
    return offPlayer::getXUIDString(player);
}

std::string Raw_GetUuid(Player *player)
{
    string uuid;
    void *data = (void*)(((uintptr_t)player) + 2832);
    SymCall("?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
        string &, void*, string *)(data, &uuid);
    return uuid;
}

string  Raw_GetRealName(Player* player)
{
    return offPlayer::getRealName(player);
}

string  Raw_GetIP(Player* player)
{
    return liteloader::getIP(*offPlayer::getNetworkIdentifier(player));
}

bool Raw_GetSneaking(Player *player)
{
    return SymCall("?isSneaking@Actor@@QEBA_NXZ",
        bool, Player*)(player);
}

bool  Raw_RuncmdAs(Player *player, const string &cmd)
{
    return liteloader::runcmdAs(player,cmd);
}

bool Raw_TeleportPlayer(Player* player, const FloatVec4 &pos)
{
    WPlayer(*player).teleport({pos.x,pos.y,pos.z},pos.dim);
    return true;
}

bool Raw_KillPlayer(Player* player)
{
    SymCall("?kill@Mob@@UEAAXXZ", void, void*)((Actor*)player);
    return true;
}

int Raw_GetPlayerPermLevel(Player* player)
{
    return (int)WPlayer(*player).getPermLvl();
}

bool Raw_SetPlayerPermLevel(Player* player, int permLevel)
{
    ((ServerPlayer*)player)->setPermissions((CommandPermissionLevel)permLevel);
    return true;
}

bool Raw_KickPlayer(Player* player, const string &msg)
{
    extern Minecraft *mc;
    auto nh = mc->getServerNetworkHandler();
    NetworkIdentifier* a = offPlayer::getNetworkIdentifier(player);
    nh->disconnectClient(*(NetworkIdentifier*)a, msg, 0);
    return true;
}

bool Raw_Tell(Player* player, const string &text, TextType type)
{
    WPlayer(*player).sendText(text,type);
    return true;
}

ItemStack* Raw_GetHand(Player* player)
{
    return (ItemStack*)&(player->getSelectedItem());
}

ItemStack* Raw_GetOffHand(Player* player)
{
    return SymCall("?getOffhandSlot@Actor@@QEBAAEBVItemStack@@XZ", ItemStack*, Player*)(player);
}

vector<ItemStack*> Raw_GetPack(Player* player)
{
    vector<ItemStack*> res;

    auto v9 = *((uintptr_t*)player + 380);
	auto v13 = (*(__int64(__fastcall**)(uintptr_t))(**(uintptr_t**)(v9 + 176) + 112i64))(*(uintptr_t*)(v9 + 176));
	for (int i = 0; i < v13; ++i)
	{
		auto v15 = (*(__int64(__fastcall**)(uintptr_t, uintptr_t))(**(uintptr_t**)(v9 + 176) + 40i64))(
			*(uintptr_t*)(v9 + 176),
			(unsigned int)i);
		res.push_back((ItemStack*)v15);
	}
    return res;
}

bool Raw_RenamePlayer(Player* player, const string &name)
{
    player->setNameTag(name);
    return true;
}
///////////////////////// Add code here ///////////////////////////
int Raw_GetScore(Player* player, const std::string &key)
{
    return 0;
}

bool Raw_SetScore(Player* player, const std::string &key, int value)
{
    return false;
}

bool Raw_AddScore(Player* player, const std::string &key, int value)
{
    return false;
}

bool Raw_RemoveScore(Player* player, const std::string &key)
{
    return false;
}

bool Raw_SetScoreBoard(Player *player, std::string title, const std::vector<std::pair<std::string,int>> &data)
{
    return false;
}

bool Raw_RemoveScoreBoard(Player *player)
{
    return false;
}

bool Raw_SetBossBar(Player *player, std::string title, float percent)
{
    return false;
}

bool Raw_RemoveBossBar(Player *player)
{
    return false;
}

vector<Player*> Raw_GetOnlinePlayers()
{
    return liteloader::getAllPlayers();
}

bool Raw_IsPlayerValid(Player *player)
{
    auto playerList = Raw_GetOnlinePlayers();
    for(auto p : playerList)
        if(p == player)
            return true;
    return false;
}