#pragma once
#include "ScriptX.h"
using namespace script;

//////////////////// APIs ////////////////////

Local<Value> GetPlayer(const Arguments& args);
Local<Value> GetOnlinePlayers(const Arguments& args);

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

    static Local<Object> newPlayer(Player *p)
    {
        auto newp = new PlayerClass(p);
        return newp->getScriptObject();
    }
    static Local<Object> newPlayer(WPlayer p)
    {
        return PlayerClass::newPlayer(p.v);
    }
    static Player* extractPlayer(Local<Value> v)
    {
        if(EngineScope::currentEngine()->isInstanceOf<PlayerClass>(v))
            return EngineScope::currentEngine()->getNativeInstance<PlayerClass>(v)->get();
        else
            return nullptr;
    }

    Local<Value> getName();
    Local<Value> getPos();
    Local<Value> getRealName();
    Local<Value> getXuid();
    Local<Value> getIP();

    Local<Value> isOP(const Arguments& args);
    Local<Value> getPlayerPermLevel(const Arguments& args);
    Local<Value> setPlayerPermLevel(const Arguments& args);

    Local<Value> runcmdAs(const Arguments& args);
    Local<Value> teleport(const Arguments& args);
    Local<Value> kill(const Arguments& args);
    Local<Value> kick(const Arguments& args);
    Local<Value> tell(const Arguments& args);
    Local<Value> getHand(const Arguments& args);
    Local<Value> rename(const Arguments& args);
};