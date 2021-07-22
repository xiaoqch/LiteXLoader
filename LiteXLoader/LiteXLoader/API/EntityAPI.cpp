#include "APIHelp.h"
#include "BaseAPI.h"
#include "EntityAPI.h"
#include "PlayerAPI.h"
#include <Kernel/Entity.h>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<EntityClass> EntityClassBuilder =
    defineClass<EntityClass>("LXL_Entity")
        .constructor(nullptr)
        .instanceProperty("name", &EntityClass::getName)
        .instanceProperty("type", &EntityClass::getType)
        .instanceProperty("id", &EntityClass::getId)
        .instanceProperty("pos", &EntityClass::getPos)
        .instanceProperty("maxHealth", &EntityClass::getMaxHealth)
        .instanceProperty("health", &EntityClass::getHealth)
        .instanceProperty("inAir", &EntityClass::getInAir)

        .instanceFunction("teleport", &EntityClass::teleport)
        .instanceFunction("kill", &EntityClass::kill)
        .instanceFunction("toPlayer", &EntityClass::toPlayer)
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
void EntityClass::set(Actor* actor)
{
    __try
    {
        id = actor->getUniqueID();
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        isValid = false;
    }
}

Actor* EntityClass::get()
{
    if (!isValid)
        return nullptr;
    else
        return SymCall("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z"
        , Actor*, Level*, ActorUniqueID, bool)(mc->getLevel(), id, 0);
}

Local<Value> EntityClass::getName()
{ 
    try{
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return String::newString(Raw_GetEntityName(entity));
    }
    CATCH("Fail in getEntityName!")
}

Local<Value> EntityClass::getType()
{
    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return String::newString(Raw_GetEntityTypeName(entity));
    }
    CATCH("Fail in getEntityType!")
}

Local<Value> EntityClass::getId()
{
    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return Number::newNumber(Raw_GetEntityTypeId(entity));
    }
    CATCH("Fail in getEntityId!")
}

Local<Value> EntityClass::getPos()
{ 
    try{
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return FloatPos::newPos(Raw_GetEntityPos(entity));
    }
    CATCH("Fail in GetEntityPos!")
}

Local<Value> EntityClass::getMaxHealth()
{
    try{
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return Number::newNumber(Raw_GetMaxHealth(entity));
    }
    CATCH("Fail in GetMaxHealth!")
}

Local<Value> EntityClass::getHealth()
{
    try{
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return Number::newNumber(Raw_GetHealth(entity));
    }
    CATCH("Fail in GetHealth!")
}

Local<Value> EntityClass::getInAir()
{
    try{
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return Boolean::newBoolean(Raw_GetIsInAir(entity));
    }
    CATCH("Fail in getInAir!")
}

Local<Value> EntityClass::teleport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        FloatVec4 pos;

        if (args.size() == 1)
        {
            // FloatPos
            FloatPos* posObj = FloatPos::extractPos(args[0]);
            if (!posObj)
                return Local<Value>();
            pos = *posObj;
        }
        else if (args.size() == 4)
        {
            // number pos
            CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[2], ValueKind::kNumber);
            CHECK_ARG_TYPE(args[3], ValueKind::kNumber);

            pos.x = args[0].asNumber().toFloat();
            pos.y = args[1].asNumber().toFloat();
            pos.z = args[2].asNumber().toFloat();
            pos.dim = args[3].toInt();
        }
        else
        {
            ERROR("Wrong type of argument in teleport!");
            return Local<Value>();
        }
        
        Actor* entity = get();
        if (!entity)
            return Local<Value>();
        return Boolean::newBoolean(Raw_TeleportEntity(entity,pos));
    }
    CATCH("Fail in teleportEntity!")
}

Local<Value> EntityClass::kill(const Arguments& args)
{
    try{
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return Boolean::newBoolean(Raw_KillEntity(entity));
    }
    CATCH("Fail in killEntity!")
}

Local<Value> EntityClass::toPlayer(const Arguments& args)
{
    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        auto pl = Raw_ToPlayer(entity);
        if (!pl)
            return Local<Value>();
        else
            return PlayerClass::newPlayer(pl);
    }
    CATCH("Fail in toPlayer!")
}