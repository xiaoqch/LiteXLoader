#include "Global.h"
#include <string>

void* Raw_CreatePacket(int type);
bool Raw_SendPacket(Player *player,void *packet);

int Raw_SendFormPacket(Player* player, const std::string &data);