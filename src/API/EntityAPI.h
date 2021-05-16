#pragma once
#include "ScriptX.h"
using namespace script;

//////////////////// Classes ////////////////////
class Actor;
class EntityPointer : public ScriptClass
{
public:
	Actor *pointer;

	explicit EntityPointer(Actor *p)
        :ScriptClass(ScriptClass::ConstructFromCpp<EntityPointer>{}),pointer(p)
    {}
    Actor *get()
    {
        return pointer;
    }
};

//////////////////// APIs ////////////////////

Local<Value> GetEntityName(const Arguments& args);
Local<Value> GetEntityPos(const Arguments& args);
Local<Value> TeleportEntity(const Arguments& args);
Local<Value> KillEntity(const Arguments& args);