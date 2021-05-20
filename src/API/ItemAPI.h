#pragma once
#include <string>
#include "ScriptX.h"
using namespace script;

//////////////////// APIs ////////////////////

Local<Value> SetLore(const Arguments& args);


//////////////////// Classes ////////////////////
class ItemStack;
class ItemClass : public ScriptClass
{
private:
    ItemStack *item;

    // Pre data
    std::string name,customName;
    int count;

public:
	explicit ItemClass(ItemStack *p);

    ItemStack *get()
    {
        return item;
    }

    static Local<Object> newItem(ItemStack *p)
    {
        auto newp = new ItemClass(p);
        return newp->getScriptObject();
    }
    static ItemStack* extractItem(Local<Value> v)
    {
        if(EngineScope::currentEngine()->isInstanceOf<ItemClass>(v))
            return EngineScope::currentEngine()->getNativeInstance<ItemClass>(v)->get();
        else
            return nullptr;
    }

    Local<Value> getName();
    Local<Value> getCustomName();
    Local<Value> getCount();

    Local<Value> setLore(const Arguments& args);
};