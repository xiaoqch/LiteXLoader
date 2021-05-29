#pragma once
#include "ScriptX.h"
using namespace script;

//////////////////// Classes ////////////////////
class Player;
class PlayerClass : public ScriptClass
{
private:
    Player *player;
public:
	explicit PlayerClass(Player *p)
        :ScriptClass(ScriptClass::ConstructFromCpp<PlayerClass>{}),player(p)
    {}
    Player *get()
    {
        return player;
    }

    static Local<Object> newPlayer(Player *p);
    static Local<Object> newPlayer(WPlayer p);
    static Player* extractPlayer(Local<Value> v);

    Local<Value> getName();
    Local<Value> getPos();
    Local<Value> getRealName();
    Local<Value> getXuid();
    Local<Value> getIP();
    Local<Value> getMaxHealth();
    Local<Value> getHealth();
    Local<Value> getInAir();

    Local<Value> isOP(const Arguments& args);
    Local<Value> getPlayerPermLevel(const Arguments& args);
    Local<Value> setPlayerPermLevel(const Arguments& args);

    Local<Value> runcmdAs(const Arguments& args);
    Local<Value> teleport(const Arguments& args);
    Local<Value> kill(const Arguments& args);
    Local<Value> kick(const Arguments& args);
    Local<Value> tell(const Arguments& args);
    Local<Value> getHand(const Arguments& args);
    Local<Value> getPack(const Arguments& args);
    Local<Value> rename(const Arguments& args);

    Local<Value> sendSimpleForm(const Arguments& args);
    Local<Value> sendModelForm(const Arguments& args);
    Local<Value> sendForm(const Arguments& args);

    Local<Value> setExtraData(const Arguments& args);
    Local<Value> getExtraData(const Arguments& args);
    Local<Value> delExtraData(const Arguments& args);
};

//////////////////// APIs ////////////////////

Local<Value> GetPlayer(const Arguments& args);
Local<Value> GetOnlinePlayers(const Arguments& args);