#pragma once
#include "ScriptX.h"
using namespace script;

//////////////////// Classes ////////////////////
class ItemStack;
class ItemPointer : public ScriptClass
{
public:
	ItemStack *pointer;

	explicit ItemPointer(ItemStack *p)
        :ScriptClass(ScriptClass::ConstructFromCpp<ItemPointer>{}),pointer(p)
    {}
    ItemStack *get()
    {
        return pointer;
    }
};

//////////////////// APIs ////////////////////

Local<Value> GetItemName(const Arguments& args);
Local<Value> GetCustomName(const Arguments& args);
Local<Value> GetCount(const Arguments& args);
Local<Value> SetLore(const Arguments& args);