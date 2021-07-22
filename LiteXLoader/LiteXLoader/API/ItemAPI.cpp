#include "APIHelp.h"
#include "ItemAPI.h"
#include <Kernel/Item.h>
#include <vector>
#include <string>
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<ItemClass> ItemClassBuilder =
    defineClass<ItemClass>("LXL_Item")
        .constructor(nullptr)
        .instanceProperty("name", &ItemClass::getName)
        .instanceProperty("type", &ItemClass::getType)
        .instanceProperty("id", &ItemClass::getId)
        .instanceProperty("count", &ItemClass::getCount)
        .instanceProperty("aux", &ItemClass::getAux)

        .instanceFunction("isNull", &ItemClass::isNull)
        .instanceFunction("setLore", &ItemClass::setLore)
        .build();


//////////////////// Classes ////////////////////

ItemClass::ItemClass(ItemStack *p)
    :ScriptClass(ScriptClass::ConstructFromCpp<ItemClass>{}),item(p)
{
    preloadData();
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
void ItemClass::preloadData()
{
    name = Raw_GetCustomName(item);
    if (name.empty())
        name = Raw_GetItemName(item);

    type = Raw_GetItemTypeName(item);
    id = Raw_GetItemId(item);
    count = Raw_GetCount(item);
    aux = Raw_GetItemAux(item);
}

Local<Value> ItemClass::getName()
{ 
    try{
        //已预加载
        return String::newString(name);
    }
    CATCH("Fail in GetItemName!")
}

Local<Value> ItemClass::getType()
{
    try{
        //已预加载
        return String::newString(type);
    }
    CATCH("Fail in GetType!")
}

Local<Value> ItemClass::getId()
{
    try {
        //已预加载
        return Number::newNumber(id);
    }
    CATCH("Fail in GetType!")
}

Local<Value> ItemClass::getCount()
{
    try{
        //已预加载
        return Number::newNumber(count);
    }
    CATCH("Fail in GetCount!")
}

Local<Value> ItemClass::getAux()
{
    try{
        //已预加载
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