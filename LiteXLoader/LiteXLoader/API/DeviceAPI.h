#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

//////////////////// Classes ////////////////////
class Player;
class DeviceClass : public ScriptClass
{
private:
    ActorUniqueID id;
    bool isValid = true;

public:
    explicit DeviceClass(Player* p)
        :ScriptClass(ScriptClass::ConstructFromCpp<DeviceClass>{})
    {
        setPlayer(p);
    }

    void setPlayer(Player* player);
    Player* getPlayer();


    static Local<Object> newDevice(Player* p);

    Local<Value> getAvgPing();
    Local<Value> getAvgPacketLoss();
    Local<Value> getOs();
    Local<Value> getClientId();
};