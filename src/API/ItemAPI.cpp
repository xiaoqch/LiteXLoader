#include "APIhelp.h"
#include "ItemAPI.h"
#include "../Kernel/Item.h"
#include <vector>
#include <string>
using namespace script;

ItemClass::ItemClass(ItemStack *p)
    :ScriptClass(ScriptClass::ConstructFromCpp<ItemClass>{}),item(p)
{
    name = Raw_GetItemName(item);
    customName = Raw_GetCustomName(item);
    count = Raw_GetCount(item);
}

Local<Value> ItemClass::getName()
{ 
    try{
        //return String::newString(Raw_GetItemName(item));
        return String::newString(name);
    }
    CATCH("Fail in GetItemName!")
}

Local<Value> ItemClass::getCustomName()
{
    try{
        //return String::newString(Raw_GetCustomName(item));
        return String::newString(customName);
    }
    CATCH("Fail in GetCustomName!")
}

Local<Value> ItemClass::getCount()
{
    try{
        //return Number::newNumber(Raw_GetCount(item));
        return Number::newNumber(count);
    }
    CATCH("Fail in GetCount!")
}

Local<Value> ItemClass::setLore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kArray)

    try{
        auto arr = args[0].asArray();
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
    CATCH("Fail in SetLore!")
}