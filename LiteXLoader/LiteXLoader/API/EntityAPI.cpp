#include "APIHelp.h"
#include "BaseAPI.h"
#include "EntityAPI.h"
#include "PlayerAPI.h"
#include "NbtAPI.h"
#include <Kernel/Entity.h>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<EntityClass> EntityClassBuilder =
    defineClass<EntityClass>("LXL_Entity")
        .constructor(nullptr)
        .instanceFunction("getRawPtr", &EntityClass::getRawPtr)

        .instanceProperty("name", &EntityClass::getName)
        .instanceProperty("type", &EntityClass::getType)
        .instanceProperty("id", &EntityClass::getId)
        .instanceProperty("pos", &EntityClass::getPos)
        .instanceProperty("maxHealth", &EntityClass::getMaxHealth)
        .instanceProperty("health", &EntityClass::getHealth)
        .instanceProperty("inAir", &EntityClass::getInAir)
        .instanceProperty("speed",&EntityClass::getSpeed)

        .instanceFunction("teleport", &EntityClass::teleport)
        .instanceFunction("kill", &EntityClass::kill)
        .instanceFunction("isPlayer", &EntityClass::isPlayer)
        .instanceFunction("toPlayer", &EntityClass::toPlayer)
        .instanceFunction("setOnFire",&EntityClass::setOnFire)
        .instanceFunction("setTag", &EntityClass::setTag)
        .instanceFunction("getTag", &EntityClass::getTag)
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
        return Raw_GetEntityByUniqueId(id);
}

Local<Value> EntityClass::getRawPtr(const Arguments& args)
{
    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();
        else
            return Number::newNumber((intptr_t)entity);
    }
    CATCH("Fail in getRawPtr!")
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

Local<Value> EntityClass::getSpeed()
{
    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return Number::newNumber(Raw_GetSpeed(entity));
    }
    CATCH("Fail in getSpeed!")
}

Local<Value> EntityClass::teleport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);
    if (args.size() == 4)
    {
        CHECK_ARG_TYPE(args[0], ValueKind::kNumber);
        CHECK_ARG_TYPE(args[1], ValueKind::kNumber);
        CHECK_ARG_TYPE(args[2], ValueKind::kNumber);
        CHECK_ARG_TYPE(args[3], ValueKind::kNumber);
    }
    
    try{
        FloatVec4 pos;

        if (args.size() == 1)
        {
            // FloatPos
            FloatPos* posObj = FloatPos::extractPos(args[0]);
            if (posObj)
            {
                pos = *posObj;
            }
            else
            {
                ERROR("Wrong type of argument in teleport!");
                return Local<Value>();
            }
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
            ERROR("Wrong number of arguments in teleport!");
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

Local<Value> EntityClass::isPlayer(const Arguments& args)
{
    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return Boolean::newBoolean(Raw_IsPlayer(entity));
    }
    CATCH("Fail in isPlayer!")
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

Local<Value> EntityClass::setOnFire(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);

    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        int time = args[0].toInt();
        bool result = Raw_SetOnFire(entity, time);
        return Boolean::newBoolean(result);
    }
    CATCH("Fail in setOnFire!")
}

Local<Value> EntityClass::getTag(const Arguments& args)
{
    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        return NbtCompound::newNBT(Tag::fromActor(entity));
    }
    CATCH("Fail in getTag!")
}

Local<Value> EntityClass::setTag(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1);

    try {
        Actor* entity = get();
        if (!entity)
            return Local<Value>();

        auto nbt = NbtCompound::extractNBT(args[0]);
        if (!nbt)
            return Local<Value>();    //Null

        nbt->setActor(entity);
        return Boolean::newBoolean(true);
    }
    CATCH("Fail in setTag!")
}