#include "APIhelp.h"
#include "EntityAPI.h"
#include "../Kernel/Entity.h"
using namespace script;

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