#include "APIhelp.h"
#include "EntityAPI.h"
#include "../Kernel/Entity.h"
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<EntityClass> EntityClassBuilder =
    defineClass<EntityClass>("Entity")
        .constructor(nullptr)
        .instanceProperty("name", &EntityClass::getName)
        .instanceProperty("pos", &EntityClass::getPos)
        .instanceProperty("maxHealth", &EntityClass::getMaxHealth)
        .instanceProperty("health", &EntityClass::getHealth)

        .instanceFunction("teleport", &EntityClass::teleport)
        .instanceFunction("kill", &EntityClass::kill)
        .build();


//////////////////// Classes ////////////////////

//生成函数
Local<Object> EntityClass::newEntity(Actor *p)
{
    auto newp = new EntityClass(p);
    return newp->getScriptObject();
}
Local<Object> EntityClass::newEntity(WActor p)
{
    return EntityClass::newEntity(p.v);
}
Actor* EntityClass::extractEntity(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<EntityClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<EntityClass>(v)->get();
    else
        return nullptr;
}

//成员函数
Local<Value> EntityClass::getName()
{ 
    try{
        return String::newString(Raw_GetEntityName(entity));
    }
    CATCH("Fail in GetEntityName!")
}

Local<Value> EntityClass::getPos()
{ 
    try{
        return FloatPos::newPos(Raw_GetEntityPos(entity));
    }
    CATCH("Fail in GetEntityPos!")
}

Local<Value> EntityClass::getMaxHealth()
{
    try{
        return Number::newNumber(Raw_GetMaxHealth(entity));
    }
    CATCH("Fail in GetMaxHealth!")
}

Local<Value> EntityClass::getHealth()
{
    try{
        return Number::newNumber(Raw_GetHealth(entity));
    }
    CATCH("Fail in GetHealth!")
}

Local<Value> EntityClass::teleport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        FloatPos *pos = FloatPos::extractPos(args[0]);
        if(!pos)
            return Local<Value>();
        
        return Boolean::newBoolean(Raw_TeleportEntity(entity,*pos));
    }
    CATCH("Fail in TeleportEntity!")
}

Local<Value> EntityClass::kill(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_KillEntity(entity));
    }
    CATCH("Fail in KillEntity!")
}