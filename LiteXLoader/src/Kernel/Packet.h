#include "Global.h"
#include <string>

void* Raw_CreatePacket(int type);
bool Raw_SendPacket(Player *player,void *packet);

int Raw_SendFormPacket(Player* player, const std::string &data);
bool Raw_SendTransferPacket(Player* player, const string& address, short port);