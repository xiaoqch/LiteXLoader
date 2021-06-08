#include "Packet.h"
#include "SymbolHelper.h"
using namespace std;

void* Raw_CreatePacket(int type)
{
    unsigned long long packet[2];
    SymCall("?createPacket@MinecraftPackets@@SA?AV?$shared_ptr@VPacket@@@std@@W4MinecraftPacketIds@@@Z",
	      void *,void*,int)(packet, type);
	return (void*)*packet;
}

bool Raw_SendPacket(Player *player,void *packet)
{
    ((ServerPlayer*)player)->sendNetworkPacket(*(Packet*)packet);
    return true;
}

int Raw_SendFormPacket(Player* player, const string &data)
{
    static int id = 1;
	++id;

    void *packet = Raw_CreatePacket(100);   //表单数据包
    dAccess<unsigned>(packet, 48) = id;
    dAccess<string>(packet, 56) = data;

    Raw_SendPacket(player,packet);
    return id;
}

bool Raw_SendTransferPacket(Player* player, const string& address, short port)
{
    void *packet = Raw_CreatePacket(85);    //跨服传送数据包
    dAccess<string>(packet, 48) = address;
    dAccess<short>(packet, 80) = port;

    Raw_SendPacket(player, packet);
    return true;
}

bool Raw_SendSetDisplayObjectivePacket(Player* player, const string& title, const string& name)
{
    void* packet = Raw_CreatePacket(107);   //显示侧边栏数据包
    dAccess<string>(packet, 48) = "sidebar";
    dAccess<string>(packet, 80) = name;
    dAccess<string>(packet, 112) = title;
    dAccess<string>(packet, 144) = "dummy";
    dAccess<char>(packet, 176) = 0;
    
    return Raw_SendPacket(player, packet);
}

bool Raw_SendSetScorePacket(Player* player, char type, const vector<ScorePacketInfo>& data)
{
    void* packet = Raw_CreatePacket(108);   //修改侧边栏数据包
    dAccess<char>(packet, 48) = type;   //set
    dAccess<vector<ScorePacketInfo>>(packet, 56) = data;

    return Raw_SendPacket(player, packet);
}

bool Raw_SendBossEventPacket(Player* player, string name, float percent, int type)
{
    void* packet = Raw_CreatePacket(74);   //Boss事件数据包
    dAccess<const ActorUniqueID*>(packet, 56) = dAccess<const ActorUniqueID*>(packet, 64) = & player->getUniqueID();
    dAccess<int>(packet, 72) = type;    //0显示, 1更新, 2隐藏
    dAccess<string>(packet, 80) = name;
    dAccess<float>(packet, 112) = percent;
   
    Raw_SendPacket(player, packet);
}