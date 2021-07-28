#include "Global.h"
#include "Device.h"
#include "Player.h"
#include <Engine/LocalShareData.h>
using namespace std;

class NetworkHandler;

int Raw_GetAvgPing(Player* player)
{
    //########### fix here ###########
    /*auto netid = offPlayer::getNetworkIdentifier(player);
    auto nwpeer = SymCall("?getPeerForUser@NetworkHandler@@QEAAPEAVNetworkPeer@@AEBVNetworkIdentifier@@@Z"
        , NetworkPeer*, NetworkHandler*, NetworkIdentifier*)(LocateService<Minecraft>()->getNetworkHandler(), netid);
    auto nwstatus = nwpeer->getNetworkStatus();
    return nwstatus.avgping;*/
    return 20;
}

float Raw_GetAvgPacketloss(Player* player)
{
    /*auto netid = offPlayer::getNetworkIdentifier(player);
    auto nwpeer = SymCall("?getPeerForUser@NetworkHandler@@QEAAPEAVNetworkPeer@@AEBVNetworkIdentifier@@@Z"
        , NetworkPeer*, NetworkHandler*, NetworkIdentifier*)(LocateService<Minecraft>()->getNetworkHandler(), netid);
    auto nwstatus = nwpeer->getNetworkStatus();
    return nwstatus.avgpacketloss;*/
    return 0.0;
}

std::string Raw_GetOs(Player* player)
{
    try
    {
        return Raw_GetOsNameByType(localShareData->deviceInfoRecord.at((uintptr_t)player).DeviceOS);
    }
    catch (...)
    {
        return "Unknown";
    }
}

std::string Raw_GetClientId(Player* player)
{
    try
    {
        return localShareData->deviceInfoRecord.at((uintptr_t)player).DeviceId;
    }
    catch (...)
    {
        return "";
    }
}

std::string Raw_GetOsNameByType(int type)
{
    string os = "";
    switch (type)
    {
    case 1:
        os = "Android";
        break;
    case 2:
        os = "iOS";
        break;
    case 3:
        os = "OSX";
        break;
    case 4:
        os = "Amazon";
        break;
    case 5:
        os = "GearVR";
        break;
    case 6:
        os = "Hololens";
        break;
    case 7:
        os = "Windows10";
        break;
    case 8:
        os = "Win32";
        break;
    case 10:
        os = "TVOS";
        break;
    case 11:
        os = "PlayStation";
        break;
    case 12:
        os = "Nintendo";
        break;
    case 13:
        os = "Xbox";
        break;
    case 14:
        os = "WindowsPhone";
        break;
    default:
        os = "Unknown";
        break;
    }
    return os;
}