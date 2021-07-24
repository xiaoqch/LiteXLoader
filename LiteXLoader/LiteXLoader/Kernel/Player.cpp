#include "Global.h"
#include "Player.h"
#include "Packet.h"
#include "SymbolHelper.h"
#include "ThirdParty.h"
#include "Global.h"
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

string Raw_GetXuid(Player* player)
{
    return offPlayer::getXUIDString(player);
}

std::string Raw_GetUuid(Player *player)
{
    string uuid;
    SymCall("?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
        string *, void*, string *)((unsigned*)((uintptr_t)player+3000), &uuid);
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

int Raw_GetPlayerPermLevel(Player* player)
{
    return (int)WPlayer(*player).getPermLvl();
}

int Raw_GetGameMode(Player* player)
{
    return SymCall("?getPlayerGameType@Player@@QEBA?AW4GameType@@XZ", unsigned short, Player*)(player);
}

bool Raw_GetSneaking(Player *player)
{
    return SymCall("?isSneaking@Actor@@QEBA_NXZ", bool, Player*)(player);
}

bool  Raw_RuncmdAs(Player *player, const string &cmd)
{
    return liteloader::runcmdAs(player,cmd);
}

bool Raw_TeleportPlayer(Player* player, const FloatVec4 &pos)
{
    WPlayer(*player).teleport({pos.x,pos.y,pos.z}  ,pos.dim);
    return true;
}

bool Raw_KillPlayer(Player* player)
{
    SymCall("?kill@Mob@@UEAAXXZ", void, void*)((Actor*)player);
    return true;
}

bool Raw_SetPlayerPermLevel(Player* player, int permLevel)
{
    ((ServerPlayer*)player)->setPermissions((CommandPermissionLevel)permLevel);
    return true;
}

bool Raw_SetGameMode(Player* player, int gameMode)
{
    SymCall("?setPlayerGameType@Player@@UEAAXW4GameType@@@Z", void, Player*, unsigned short)(player, (unsigned short)gameMode);
    return true;        //############ 加上发包给客户端 ############
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

bool Raw_Broadcast(const std::string& text, TextType type)
{
    auto playerList = Raw_GetOnlinePlayers();
    for (auto player : playerList)
        Raw_Tell(player, text, type);
    return true;
}

ItemStack* Raw_GetHand(Player* player)
{
    return (ItemStack*)&(player->getSelectedItem());
}

bool Raw_GetAllItems(Player* player, ItemStack** hand, ItemStack** offHand, vector<ItemStack*>* inventory,
    vector<ItemStack*>* armor, vector<ItemStack*>* endChest)
{
    //Hand
    *hand = (ItemStack*)&(player->getSelectedItem());

    //OffHand
    *offHand = SymCall("?getOffhandSlot@Actor@@QEBAAEBVItemStack@@XZ", ItemStack*, Player*)(player);

    //Inventory
    Container* container = SymCall("?getInventory@Player@@QEAAAEAVContainer@@XZ", Container*, Player*)(player);
    auto slot = container->getSlots();
    for (auto& item : slot)
        inventory->push_back((ItemStack*)item);
    
    //Armor
    container = dAccess<Container*>(player, 1648);
    slot = container->getSlots();
    for (auto& item : slot)
        armor->push_back((ItemStack*)item);

    //EndChest
    container = dAccess<Container*>(player, 4360);
    slot = container->getSlots();
    for (auto& item : slot)
        armor->push_back((ItemStack*)item);

    return true;
}

bool Raw_RenamePlayer(Player* player, const string &name)
{
    player->setNameTag(name);
    return true;
}

bool Raw_AddLevel(Player* player, int level)
{
    SymCall("?addLevels@Player@@UEAAXH@Z", void, Player*, int)(player, level);
    return true;
}

bool Raw_TransServer(Player* player, const std::string& server, short port)
{
    return Raw_SendTransferPacket(player, server, port);
}

bool Raw_CrashPlayer(Player* player)
{
    return Raw_SendCrashClientPacket(player);
}

int Raw_GetScore(Player* player, const std::string &key)
{
    Objective* obj = g_scoreboard->getObjective(key);
    if (obj)
    {
        auto id = g_scoreboard->getScoreboardId(*(Actor*)player);
        auto score = obj->getPlayerScore(id);
        return score.getCount();
    }
    return 0;
}

bool Raw_SetScore(Player* player, const std::string &key, int value)
{
    Objective* obj = g_scoreboard->getObjective(key);
    if (obj)
    {
        bool a1 = true;
        bool &pa = a1;
        g_scoreboard->modifyPlayerScore(pa,g_scoreboard->getScoreboardId(*(Actor*)player), *obj, value, 0);   //Set
        return true;
    }
    return false;
}

bool Raw_AddScore(Player* player, const std::string &key, int value)
{
    Objective* obj = g_scoreboard->getObjective(key);
    if (obj)
    {
        bool a1 = true;
        bool& pa = a1;
        g_scoreboard->modifyPlayerScore(pa, g_scoreboard->getScoreboardId(*(Actor*)player), *obj, value, 1);   //Add
        return true;
    }
    return false;
}

bool Raw_RemoveScore(Player* player, const std::string &key)
{
    Objective* obj = g_scoreboard->getObjective(key);
    if (obj)
    {
        bool a1 = true;
        bool& pa = a1;
        g_scoreboard->modifyPlayerScore(pa, g_scoreboard->getScoreboardId(*(Actor*)player), *obj, 0, 2);   //Remove
        return true;
    }
    return false;
}

bool Raw_SetSidebar(Player *player, std::string title, const std::vector<std::pair<std::string,int>> &data)
{
    Raw_SendSetDisplayObjectivePacket(player, title, "name");

    vector<ScorePacketInfo> info;
    for (auto& x : data)
    {
        ScorePacketInfo i(g_scoreboard->createScoreboardId(x.first),x.second,x.first);
        info.push_back(i);
    }

    return Raw_SendSetScorePacket(player, 0, info);    //set
}

bool Raw_RemoveSidebar(Player *player)
{
    return Raw_SendSetDisplayObjectivePacket(player, "", "");
}

bool Raw_SetBossBar(Player *player, std::string title, float percent)
{
    return Raw_SendBossEventPacket(player, title, percent, 0);
}

bool Raw_RemoveBossBar(Player *player)
{
    return Raw_SendBossEventPacket(player, "", 0, 2);;
}

vector<Player*> Raw_GetOnlinePlayers()
{
    try
    {
        return isServerStarted ? liteloader::getAllPlayers() : vector<Player*>();
    }
    catch (const seh_exception& e)
    {
        return vector<Player*>();
    }
}

bool Raw_IsPlayerValid(Player *player)
{
    auto playerList = Raw_GetOnlinePlayers();
    for(auto p : playerList)
        if(p == player)
            return true;
    return false;
}

int Raw_GetPlayerDimId(Player* player)
{
    return WPlayer(*player).getDimID();
}