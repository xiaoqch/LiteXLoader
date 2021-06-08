#include "Global.h"
#include <string>
#include <vector>

class ScorePacketInfo;

void* Raw_CreatePacket(int type);
bool Raw_SendPacket(Player *player,void *packet);

int Raw_SendFormPacket(Player* player, const std::string &data);
bool Raw_SendTransferPacket(Player* player, const string& address, short port);
bool Raw_SendSetDisplayObjectivePacket(Player* player, const string& title, const string& name);
bool Raw_SendSetScorePacket(Player* player, char type, const vector<ScorePacketInfo>& data);

bool Raw_SendBossEventPacket(Player* player, string name, float percent, int type);