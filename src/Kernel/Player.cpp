#include "Player.h"
using namespace std;

string inline Raw_GetPlayerName(Player* player)
{
    return player->getNameTag();
}

FloatVec4 Raw_GetPlayerPos(Player* player)
{
    auto pos = player->getPos();
    return {pos.x,pos.y,pos.z,WPlayer(*player).getDimID()};
}

string inline Raw_GetXuid(Player* player)
{
    return offPlayer::getXUIDString(player);
}

string inline Raw_GetRealName(Player* player)
{
    return offPlayer::getRealName(player);
}

string inline Raw_GetIP(Player* player)
{
    return liteloader::getIP(*offPlayer::getNetworkIdentifier(player));
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

int inline Raw_GetPlayerPermLevel(Player* player)
{
    return (int)WPlayer(*player).getPermLvl();
}

bool inline Raw_SetPlayerPermLevel(Player* player, int permLevel)
{
    ((ServerPlayer*)player)->setPermissions((CommandPermissionLevel)permLevel);
    return true;
}

bool Raw_KickPlayer(Player* player, const string &msg)
{
    Minecraft *mc;
    auto nh = mc->getServerNetworkHandler();
    NetworkIdentifier* a = offPlayer::getNetworkIdentifier(player);
    nh->disconnectClient(*(NetworkIdentifier*)a, msg, 0);
    return true;
}

bool inline Raw_Tell(Player* player, const string &text, TextType type)
{
    WPlayer(*player).sendText(text,type);
    return true;
}

inline ItemStack* Raw_GetHand(Player* player)
{
    return (ItemStack*)&(player->getSelectedItem());
}

bool inline Raw_RenamePlayer(Player* player, const string &name)
{
    player->setNameTag(name);
    return true;
}

vector<Player*> inline Raw_GetPlayerList()
{
    return liteloader::getAllPlayers();
}