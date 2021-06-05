#pragma once
#include <ScriptX/ScriptX.h>
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
    Local<Value> getType();
    Local<Value> getPos();
    Local<Value> getMaxHealth();
    Local<Value> getHealth();
    Local<Value> getInAir();

    Local<Value> teleport(const Arguments& args);
    Local<Value> kill(const Arguments& args);
};