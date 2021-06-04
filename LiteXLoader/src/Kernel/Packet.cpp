#include "Packet.h"
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
    static int id = 0;
	id = 255 + id * 127;    //############## ???? ##############

    void *packet = Raw_CreatePacket(100);   // Form Packet
    dAccess<unsigned,48>(packet) = id;
    dAccess<string,56>(packet) = data;

    Raw_SendPacket(player,packet);
    return id;
}