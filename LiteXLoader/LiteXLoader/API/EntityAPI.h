#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;

//////////////////// Classes ////////////////////
class Actor;
class EntityClass : public ScriptClass
{
private:
    ActorUniqueID id;
    bool isValid = true;

public:
	explicit EntityClass(Actor *p)
        :ScriptClass(ScriptClass::ConstructFromCpp<EntityClass>{})
    {
        set(p);
    }

    void set(Actor* actor);
    Actor* get();

    static Local<Object> newEntity(Actor *p);
    static Local<Object> newEntity(WActor p);
    static Actor* extractEntity(Local<Value> v);
    Local<Value> getRawPtr(const Arguments& args);

    Local<Value> getName();
    Local<Value> getType();
    Local<Value> getId();
    Local<Value> getPos();
    Local<Value> getMaxHealth();
    Local<Value> getHealth();
    Local<Value> getInAir();
    Local<Value> getSpeed();

    Local<Value> teleport(const Arguments& args);
    Local<Value> kill(const Arguments& args);
    Local<Value> isPlayer(const Arguments& args);
    Local<Value> toPlayer(const Arguments& args);
    Local<Value> setOnFire(const Arguments& args);
    
    Local<Value> getTag(const Arguments& args);
    Local<Value> setTag(const Arguments& args);
};