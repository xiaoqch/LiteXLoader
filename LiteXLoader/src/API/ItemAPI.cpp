#include "APIhelp.h"
#include "ItemAPI.h"
#include <Kernel/Item.h>
#include <vector>
#include <string>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<ItemClass> ItemClassBuilder =
    defineClass<ItemClass>("Item")
        .constructor(nullptr)
        .instanceProperty("name", &ItemClass::getName)
        .instanceProperty("customName", &ItemClass::getCustomName)
        .instanceProperty("count", &ItemClass::getCount)
        .instanceProperty("aux", &ItemClass::getAux)

        .instanceFunction("setLore", &ItemClass::setLore)
        .build();


//////////////////// Classes ////////////////////

ItemClass::ItemClass(ItemStack *p)
    :ScriptClass(ScriptClass::ConstructFromCpp<ItemClass>{}),item(p)
{
    name = Raw_GetItemFullName(item);

    customName = Raw_GetCustomName(item);
    if(customName.empty())
        customName = Raw_GetItemName(item);
    
    count = Raw_GetCount(item);
    aux = Raw_GetItemAux(item);
}

//生成函数
Local<Object> ItemClass::newItem(ItemStack *p)
{
    auto newp = new ItemClass(p);
    return newp->getScriptObject();
}
ItemStack* ItemClass::extractItem(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<ItemClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<ItemClass>(v)->get();
    else
        return nullptr;
}

//成员函数
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

Local<Value> ItemClass::getAux()
{
    try{
        //return Number::newNumber(Raw_GetItemAux(item));
        return Number::newNumber(aux);
    }
    CATCH("Fail in GetAux!")
}

Local<Value> ItemClass::isNull(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_IsNull(item));
    }
    CATCH("Fail in IsNull!")
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