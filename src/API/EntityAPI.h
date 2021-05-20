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

    static Local<Object> newEntity(Actor *p)
    {
        auto newp = new EntityClass(p);
        return newp->getScriptObject();
    }
    static Local<Object> newEntity(WActor p)
    {
        return EntityClass::newEntity(p.v);
    }
    static Actor* extractEntity(Local<Value> v)
    {
        if(EngineScope::currentEngine()->isInstanceOf<EntityClass>(v))
            return EngineScope::currentEngine()->getNativeInstance<EntityClass>(v)->get();
        else
            return nullptr;
    }

    Local<Value> getName();
    Local<Value> getPos();

    Local<Value> teleport(const Arguments& args);
    Local<Value> kill(const Arguments& args);
};