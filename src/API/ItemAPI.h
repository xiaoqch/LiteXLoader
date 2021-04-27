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