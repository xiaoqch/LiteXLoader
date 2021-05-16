#include "APIhelp.h"
#include "ItemAPI.h"
#include "../Kernel/Item.h"
#include <vector>
#include <string>
using namespace script;

//////////////////// APIs ////////////////////

Local<Value> GetItemName(const Arguments& args)
{ 
    CHECK_ARGS_COUNT(args,1)

    try{
        ItemStack *item = ExtractItem(args[0]);
        if(item)
            return String::newString(Raw_GetItemName(item));
        
        return Local<Value>(); // Null
    }
    CATCH("Fail in GetItemName!")
}

Local<Value> GetCustomName(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        ItemStack* item = ExtractItem(args[0]);
        if(item)
        {
            return String::newString(Raw_GetCustomName(item));
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetCustomName!")
}

Local<Value> GetCount(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        ItemStack* item = ExtractItem(args[0]);
        if(item)
        {
            return Number::newNumber(Raw_GetCount(item));
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in GetCount!")
}

Local<Value> SetLore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[1],ValueKind::kArray)

    try{
        ItemStack* item = ExtractItem(args[0]);
        if(item)
        {
            auto arr = args[1].asArray();
            std::vector<std::string> lores;
            for(int i=0;i<arr.size();++i)
            {
                auto value = arr.get(i);
                if(value.getKind() == ValueKind::kString)
                    lores.push_back(value.asString().toString());
            }
            if(lores.empty())
                return Boolean::newBoolean(false);
            
            Raw_SetLore(item, lores);
            return Boolean::newBoolean(true);
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in SetLore!")
}