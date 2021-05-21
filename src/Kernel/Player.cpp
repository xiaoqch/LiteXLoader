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

string  Raw_GetRealName(Player* player)
{
    return offPlayer::getRealName(player);
}

string  Raw_GetIP(Player* player)
{
    return liteloader::getIP(*offPlayer::getNetworkIdentifier(player));
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

int  Raw_GetPlayerPermLevel(Player* player)
{
    return (int)WPlayer(*player).getPermLvl();
}

bool  Raw_SetPlayerPermLevel(Player* player, int permLevel)
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

bool  Raw_Tell(Player* player, const string &text, TextType type)
{
    WPlayer(*player).sendText(text,type);
    return true;
}

 ItemStack* Raw_GetHand(Player* player)
{
    return (ItemStack*)&(player->getSelectedItem());
}

bool  Raw_RenamePlayer(Player* player, const string &name)
{
    player->setNameTag(name);
    return true;
}

vector<Player*>  Raw_GetOnlinePlayers()
{
    return liteloader::getAllPlayers();
}