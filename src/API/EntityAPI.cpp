#include "APIhelp.h"
#include "EntityAPI.h"
#include "../Kernel/Entity.h"
using namespace script;

Local<Value> GetEntityName(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return String::newString(Raw_GetEntityName(entity));
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetEntityName!")
}

Local<Value> GetEntityPos(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return NewPos(Raw_GetEntityPos(entity));
 
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetEntityPos!")
}

Local<Value> TeleportEntity(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    
    try{
        FloatPos *pos = ExtractFloatPos(args[1]);
        if(!pos)
            return Local<Value>();
        
        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return Boolean::newBoolean(Raw_TeleportEntity(entity,*pos););

        return Local<Value>();    //Null
    }
    CATCH("Fail in TeleportEntity!")
}

Local<Value> KillEntity(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)

    try
    {
        Actor *entity = ExtractEntity(args[0]);
        if(entity)
            return Boolean::newBoolean(Raw_KillEntity(entity));

        return Local<Value>();    //Null
    }
    CATCH("Fail in KillEntity!")
}