#include "APIhelp.h"
#include "ItemAPI.h"
#include <vector>
#include <string>
using namespace script;

//////////////////// APIs ////////////////////

Local<Value> GetCustomName(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        ItemStack* item = ExtractItem(args[0]);
        if(item)
        {
            return String::newString(item->getCustomName());
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
            return Number::newNumber(WItem(*item).getCount());
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
            
            SymCall("?setCustomLore@ItemStackBase@@QEAAXAEBV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@@Z",
		        void, void*, std::vector<std::string>)(item, lores);
            return Boolean::newBoolean(true);
        }
        else
            return Local<Value>();    //Null
    }
    CATCH("Fail in SetLore!")
}