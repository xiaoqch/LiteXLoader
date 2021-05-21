#pragma once
#include "ScriptX.h"
using namespace script;

//////////////////// Classes ////////////////////
class Actor;
class EntityClass : public ScriptClass
{
private:
    Actor *entity;
public:
	explicit EntityClass(Actor *p)
        :ScriptClass(ScriptClass::ConstructFromCpp<EntityClass>{}),entity(p)
    {}
    Actor *get()
    {
        return entity;
    }

    static Local<Object> newEntity(Actor *p);
    static Local<Object> newEntity(WActor p);
    static Actor* extractEntity(Local<Value> v);

    Local<Value> getName();
    Local<Value> getPos();

    Local<Value> teleport(const Arguments& args);
    Local<Value> kill(const Arguments& args);
};