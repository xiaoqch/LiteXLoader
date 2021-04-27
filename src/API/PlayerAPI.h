#pragma once
#include "ScriptX.h"
using namespace script;

//////////////////// Classes ////////////////////
class Player;
class PlayerPointer : public ScriptClass
{
public:
	Player *pointer;

	explicit PlayerPointer(Player *p)
        :ScriptClass(ScriptClass::ConstructFromCpp<PlayerPointer>{}),pointer(p)
    {}
    Player *get()
    {
        return pointer;
    }
};

//////////////////// APIs ////////////////////

Local<Value> GetPlayer(const Arguments& args);
Local<Value> GetXuid(const Arguments& args);
Local<Value> GetRealName(const Arguments& args);
Local<Value> GetIP(const Arguments& args);

Local<Value> IsOP(const Arguments& args);
Local<Value> GetPlayerPermLevel(const Arguments& args);
Local<Value> SetPlayerPermLevel(const Arguments& args);

Local<Value> KickPlayer(const Arguments& args);
Local<Value> Tell(const Arguments& args);
Local<Value> GetHand(const Arguments& args);
Local<Value> RunCmdAs(const Arguments& args);
Local<Value> RenamePlayer(const Arguments& args);